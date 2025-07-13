import serial
import struct
import time
import numpy as np
import matplotlib.pyplot as plt
import math

# --- CONFIGURAÇÕES ---
SERIAL_PORT = 'COM8'
BAUD_RATE = 115200
NUM_DAC = 200
NUM_ADC = 100
Fundamental = 50  # Hz
Amost = 100       # Número de amostras por ciclo

# --- COMANDOS ---
CMD_RECEIVE_INT = 1
CMD_SEND_INT = 2
CMD_RECEIVE_SEN = 3
CMD_SEND_SEN = 4

# --- GERAÇÃO DE SINAIS ---

def gerar_senoide_fundamental(freq=Fundamental, num_amostras=NUM_DAC, fs=None, amp=1.0):
    if fs is None:
        fs = freq * num_amostras
    t = np.arange(num_amostras) / fs
    offset = 2048
    amplitude = amp * 2047
    senoide = offset + amplitude * np.sin(2 * np.pi * freq * t)
    senoide = np.clip(senoide, 0, 4095)
    return senoide.astype(np.uint16), senoide / 4095.0


def gerar_senoide_com_harmonicas(freqs=[50, 200, 650], amps_norm=[0.6, 0.2, 0.2], num_amostras=NUM_DAC):
    res_dac = 12
    max_dac_val = (2 ** res_dac) - 1
    offset = max_dac_val / 2.0

    menor_freq = min(freqs)
    periodo_base = 1 / menor_freq
    ts = periodo_base / num_amostras

    dac_valores = []

    for i in range(num_amostras):
        t = i * ts
        valor_senoide = 0
        for freq, amp_norm in zip(freqs, amps_norm):
            amp_dac = amp_norm * (max_dac_val / 2.0)
            valor_senoide += amp_dac * math.sin(2 * math.pi * freq * t)

        valor_total = offset + valor_senoide
        valor_total = max(0, min(valor_total, max_dac_val))
        dac_valores.append(int(round(valor_total)))

    return dac_valores

# --- ENVIO ---

def send_senoide(ser_connection):
    global Amost, Fundamental
    try:
        amp = float(input("Amplitude (0.0 a 1.0): "))
        if not 0.0 <= amp <= 1.0:
            print("Erro: amplitude fora do intervalo permitido.")
            return

        com_harmonicas = input("Deseja harmônicas? (s/n): ").strip().lower() == 's'

        freq = Fundamental
        fs = freq * Amost

        print(f"[DEBUG] Amost = {Amost} | freq = {freq} Hz | fs = {fs} Hz")

        if com_harmonicas:
            senoide_uint16 = gerar_senoide_com_harmonicas()
            print("[INFO] Senoide com harmônicas gerada.")
        else:
            senoide_uint16, _ = gerar_senoide_fundamental(freq, NUM_DAC, fs, amp)
            print("[INFO] Senoide pura (fundamental) gerada.")

        payload = struct.pack('<' + 'H' * NUM_DAC, *senoide_uint16)
        packet = struct.pack('<Bh', CMD_RECEIVE_SEN, NUM_DAC * 2) + payload

        ser_connection.write(packet)
        print(f"[OK] Senoide enviada ({NUM_DAC} amostras).")

        plt.plot(senoide_uint16)
        plt.title(f"Senoide {'com harmônicas' if com_harmonicas else 'fundamental'} (fs={fs} Hz)")
        plt.grid()
        plt.tight_layout()
        plt.show()

    except Exception as e:
        print(f"[ERRO] {e}")

# --- RECEPÇÃO ADC + FFT ---

def receive_adc(ser_connection):
    global Amost
    request = struct.pack('<Bh', CMD_SEND_SEN, 0)
    ser_connection.write(request)
    print("[INFO] Solicitando dados do ADC...")

    expected_bytes = NUM_ADC * 2
    raw = ser_connection.read(expected_bytes)

    if len(raw) < expected_bytes:
        print(f"[ERRO] Esperado {expected_bytes} bytes, recebido {len(raw)}.")
        return None, None

    formato = f'<{NUM_ADC}h'
    tensao = struct.unpack(formato, raw)

    print(f"[OK] {NUM_ADC} amostras recebidas.")

    cc = np.mean(tensao)
    tensao = np.subtract(tensao, cc)

    plt.figure(figsize=(10, 4))
    plt.plot(tensao, label='ADC (V)')
    plt.title("Sinal Amostrado pelo ADC")
    plt.xlabel("Amostra")
    plt.ylabel("Tensão (V)")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()
    plt.show()

    fs = Amost * Fundamental
    fft_adc(tensao, fs)


def fft_adc(signal, fs):
    N = len(signal)
    fft_vals = np.fft.fft(signal)
    fft_magnitude = np.abs(fft_vals) / N
    fft_magnitude = fft_magnitude[:N//2] * 2
    freqs = np.fft.fftfreq(N, 1 / fs)[:N // 2]

    max_amp = np.max(fft_magnitude)
    max_freq = freqs[np.argmax(fft_magnitude)]

    plt.figure(figsize=(10, 4))
    plt.stem(freqs, fft_magnitude, basefmt=" ")
    plt.title(f"FFT do sinal amostrado - Máx: {max_amp:.4f} em {max_freq:.1f} Hz")
    plt.xlabel("Frequência (Hz)")
    plt.ylabel("Magnitude")
    plt.grid(True)
    plt.tight_layout()
    plt.show()

# --- COMUNICAÇÃO INT ---

def send_int(ser_connection):
    global Amost
    try:
        val = int(input("Digite o número de amostras por ciclo (ex: 20~100): "))
        if not 1 <= val <= 100:
            print("Erro: valor fora do intervalo permitido.")
            return
        Amost = val
        packet = struct.pack('<Bhh', CMD_RECEIVE_INT, 2, val)
        ser_connection.write(packet)
        print(f"[OK] Amostragem ({val}) enviada.")
    except Exception as e:
        print(f"[ERRO] {e}")


def receive_int(ser_connection):
    global Amost
    packet = struct.pack('<Bh', CMD_SEND_INT, 0)
    ser_connection.write(packet)
    raw = ser_connection.read(2)
    if len(raw) == 2:
        val = struct.unpack('<h', raw)[0]
        Amost = val
        print(f"[OK] Valor recebido do microcontrolador: {val}")
    else:
        print("[ERRO] Timeout na leitura.")

# --- MAIN ---

def main():
    print("--- Terminal SCI - TMS320F28379D ---")
    try:
        with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=2) as ser:
            print(f"[OK] Porta {SERIAL_PORT} aberta.")

            while True:
                print("\n----- MENU -----")
                print("1. Enviar nº de amostragem para o microcontrolador")
                print("2. Receber nº de amostragem do microcontrolador")
                print("3. Enviar senoide para DAC")
                print("4. Receber dados do ADC e exibir")
                print("0. Sair")
                opcao = input("Escolha: ")

                if opcao == '1':
                    send_int(ser)
                elif opcao == '2':
                    receive_int(ser)
                elif opcao == '3':
                    send_senoide(ser)
                elif opcao == '4':
                    receive_adc(ser)
                elif opcao == '0':
                    print("Encerrando.")
                    break
                else:
                    print("Opção inválida.")
    except serial.SerialException as e:
        print(f"[ERRO] Não foi possível abrir a porta {SERIAL_PORT}: {e}")

if __name__ == "__main__":
    main()

