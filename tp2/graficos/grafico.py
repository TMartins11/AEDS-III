import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Lê o CSV original
df = pd.read_csv("resultados.csv", header=None, names=["instancia", "algoritmo", "tempo", "custo"])

df["tempo"] = pd.to_numeric(df["tempo"], errors="coerce")
df["custo"] = pd.to_numeric(df["custo"], errors="coerce")

instancias = df["instancia"].unique()

# Cria uma lista para o algoritmo força bruta, visto que ele não é capaz de calcular em tempo viável nenhuma instância apresentada.
brute_force_rows = []

for inst in instancias:
    brute_force_rows.append({
        "instancia": inst,
        "algoritmo": "forca_bruta",
        "tempo": 999999.0,          
        "custo": float("inf")        
    })

df = pd.concat([df, pd.DataFrame(brute_force_rows)], ignore_index=True)

# Gráfico: tempo de execução
plt.figure(figsize=(12, 6))
sns.barplot(data=df, x="instancia", y="tempo", hue="algoritmo")
plt.title("Tempo de Execução por Instância e Algoritmo (TSP)")
plt.ylabel("Tempo (segundos)")
plt.xticks(rotation=45)
plt.yscale("log")  
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