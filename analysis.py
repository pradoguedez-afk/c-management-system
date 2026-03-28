import pandas as pd
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
import numpy as np

# Leer datos
df = pd.read_csv("sales.csv")

print("\n=== DATA ===")
print(df)

# =========================
# MÉTRICAS BÁSICAS
# =========================

total_sales = df["total"].sum()
total_items = df["quantity"].sum()

print("\n💰 Total vendido:", total_sales)
print("📦 Total de productos vendidos:", total_items)

top_product = df.groupby("product")["quantity"].sum().idxmax()
print("🏆 Producto más vendido:", top_product)

# =========================
# AGRUPAR DATOS
# =========================

grouped = df.groupby("product").agg({
    "quantity": "sum",
    "total": "sum"
}).sort_values(by="quantity", ascending=False)

print("\n=== RESUMEN POR PRODUCTO ===")
print(grouped)

# =========================
# GRÁFICO 1: PRODUCTOS MÁS VENDIDOS
# =========================

grouped["quantity"].plot(kind="bar")
plt.title("Productos más vendidos")
plt.xlabel("Producto")
plt.ylabel("Cantidad vendida")
plt.xticks(rotation=45)

plt.savefig("productos_vendidos.png")
plt.close()

# =========================
# GRÁFICO 2: INGRESOS POR PRODUCTO
# =========================

grouped["total"].plot(kind="bar")
plt.title("Ingresos por producto")
plt.xlabel("Producto")
plt.ylabel("Total generado")
plt.xticks(rotation=45)

plt.savefig("Ingresos_por_producto.png")
plt.close()

# =========================
# IA BÁSICA: PREDICCIÓN
# =========================

# Crear eje X (número de venta)
df["index"] = np.arange(len(df))

# X = posición (tiempo)
X = df[["index"]]

# y = total vendido
y = df["total"]

# Crear modelo
model = LinearRegression()
model.fit(X, y)

# Predecir siguientes 5 ventas
future_index = np.arange(len(df), len(df) + 5).reshape(-1, 1)
predictions = model.predict(future_index)

print("\n🤖 PREDICCIÓN DE PRÓXIMAS VENTAS:")
for i, pred in enumerate(predictions):
    print(f"Venta futura {i+1}: {pred:.2f}")

# =========================
# GRÁFICO CON PREDICCIÓN
# =========================

import matplotlib.pyplot as plt

plt.scatter(df["index"], y, label="Datos reales")
plt.plot(df["index"], model.predict(X), label="Tendencia")

plt.scatter(future_index, predictions, label="Predicción", marker="x")

plt.legend()
plt.title("Predicción de ventas")
plt.xlabel("Número de venta")
plt.ylabel("Total")

plt.show()