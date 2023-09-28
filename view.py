import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

#número total de tempos
nt = 15

fig, ax = plt.subplots()

#função de inicialização da animação
def init():
    ax.clear()

#função de atualização da animação
def update(t):
    if t % 2 == 0:
        filename = f"samples/sample_t{t}.bin"

        data = np.fromfile(filename, dtype=np.float32)

        # tamanho da matriz
        nx, ny, nz = (50, 50, 50)
        data = data.reshape((nx, ny, nz))

        # 15 é a localização em z da fonte
        plt.contourf(data[:, :, 15], cmap='viridis', levels=10)
        plt.title(f'Tempo {t}')

# Cria a animação
ani = animation.FuncAnimation(fig, update, frames=nt, init_func=init, repeat=False, interval = 1500)

# Mostra a animação
plt.show()
