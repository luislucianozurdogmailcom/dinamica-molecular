import seaborn as sns
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

# Cargar los datos desde el archivo usando numpy
file_path          = "output/energia.dat"
file_magnetizacion = "output/magnetizacionMedia.dat"

data = np.loadtxt(file_path)
dataMagnetizacion = np.loadtxt(file_magnetizacion)

# Crear un DataFrame para facilitar el manejo de los datos
df              = pd.DataFrame(data, columns=["Energía"])
dfMagnetizacion = pd.DataFrame(dataMagnetizacion, columns=["Magnetizacion Media"])

# Configurar el estilo de Seaborn
sns.set(style="whitegrid")

# Crear la gráfica
plt.figure(figsize=(10, 6))
sns.lineplot(data=df, x=df.index, y="Energía", marker="o", color="blue")
plt.title("Gráfica de Energía", fontsize=16)
plt.xlabel("Índice", fontsize=12)
plt.ylabel("Energía", fontsize=12)

# Guardar la gráfica como PDF con 300 DPI
output_path = "output/energia_grafica.pdf"
plt.savefig(output_path, format="pdf", dpi=300)

# -------------------------------- Segundo Gráfico
# Crear la gráfica
plt.figure(figsize=(10, 6))
sns.lineplot(data=dfMagnetizacion, x=dfMagnetizacion.index, y="Magnetizacion Media", marker=".", color="blue")
plt.title("Gráfica de Magnetización", fontsize=16)
plt.xlabel("Índice", fontsize=12)
plt.ylabel("Magnetización", fontsize=12)

# Guardar la gráfica como PDF con 300 DPI
output_path = "output/magnetizacion_grafica.pdf"
plt.savefig(output_path, format="pdf", dpi=300)
