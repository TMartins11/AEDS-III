import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Lê o CSV original
df = pd.read_csv("resultados.csv", header=None, names=["instancia", "algoritmo", "tempo", "custo"])

# Converte os valores para número (só por segurança)
df["tempo"] = pd.to_numeric(df["tempo"], errors="coerce")
df["custo"] = pd.to_numeric(df["custo"], errors="coerce")

# Instâncias únicas
instancias = df["instancia"].unique()

# Cria uma lista com entradas do "forca_bruta"
brute_force_rows = []

for inst in instancias:
    brute_force_rows.append({
        "instancia": inst,
        "algoritmo": "forca_bruta",
        "tempo": 999999.0,           # Simulando um tempo "infinito"
        "custo": float("inf")        # Ou pode usar 999999 também
    })

# Adiciona ao DataFrame
df = pd.concat([df, pd.DataFrame(brute_force_rows)], ignore_index=True)

# Gráfico: tempo de execução
plt.figure(figsize=(12, 6))
sns.barplot(data=df, x="instancia", y="tempo", hue="algoritmo")
plt.title("Tempo de Execução por Instância e Algoritmo (TSP)")
plt.ylabel("Tempo (segundos)")
plt.xticks(rotation=45)
plt.yscale("log")  # Escala logarítmica pra acomodar o 'forca_bruta'
plt.tight_layout()
plt.show()

# Gráfico: custo da solução
plt.figure(figsize=(12, 6))
sns.barplot(data=df, x="instancia", y="custo", hue="algoritmo")
plt.title("Custo da Solução por Instância e Algoritmo (TSP)")
plt.ylabel("Custo da Solução")
plt.xticks(rotation=45)
plt.tight_layout()
plt.show()