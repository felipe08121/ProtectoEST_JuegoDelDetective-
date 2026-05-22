# El Caso del Detective

**Proyecto Final — Estructuras de Datos | Mayo 2026**

## ¿De qué va el juego?

Eres un detective que llega a una ciudad llena de misterio. Alguien cometió un crimen y tú debes resolverlo. El problema es que la ciudad está llena de callejones cerrados, testigos escondidos y pistas ocultas que tendrás que encontrar.

El mapa de la ciudad es una cuadrícula de 9×9 donde cada celda es una ubicación. Al inicio todo está oculto (marcado con `o`) y, a medida que te mueves, vas descubriendo calles vacías, callejones bloqueados, pistas y testigos.

### ¿Cómo se juega?

```
# # # # # # # # # # #
# o o o o o o o o o #
# o o o o o o o o o #
# o o o o I o o o o #    ← I es tu detective
# o o o o o o o o o #
# o o o o o o o o o #
# # # # # # # # # # #
```

- **Moverte:** `W` (arriba), `A` (izquierda), `S` (abajo), `D` (derecha)
- **Ver pistas recolectadas:** `T`
- **Usar una pista:** `X` (usa la última recolectada, con efecto especial)
- **Ver sospechosos:** `Z`
- **Interrogar testigo:** `I`
- **Salir:** `Q`

### Objetivo

1. Recolectar las **10 pistas** ocultas en el mapa
2. Cada pista revela un atributo del culpable (estatura, cabello, etc.)
3. Al encontrar las 10, pasas a la **fase de acusación**
4. Usas los atributos revelados para identificar al culpable entre los 8 sospechosos
5. ¡Acusa correctamente y resuelve el caso!

### Elementos del juego

| Símbolo | Significado |
|---------|------------|
| `#` | Edificio (borde — no puedes pasar) |
| `o` | Ubicación sin descubrir |
| ` ` | Calle descubierta (vacía) |
| `\|` | Callejón cerrado (no puedes pasar) |
| `I` | Tu detective (Investigador) |
| `W` | Testigo |

### Tipos de pistas y sus efectos al usar con `X`

| Pista | Efecto |
|-------|--------|
| **H** — Huella | Tu puntaje se reduce a la mitad |
| **C** — Coartada | Se eliminan 2 callejones del mapa |
| **T** — Testimonio | Tu puntaje baja a 0 o se duplica (al azar) |
| **P** — Prueba Forense | Te teletransporta a otra ubicación |

> Advertencia: Al usar una pista, esta vuelve al mapa y todas las celdas se cubren de nuevo con `o`.

### Puntaje

Cada movimiento suma 1 punto. **Menos puntos = mejor puntaje.** Si acusas mal, tu puntaje se duplica como penalización.

### Ranking

El juego guarda un ranking histórico en un archivo `ranking.json`. Si juegas varias veces con el mismo nombre, solo se guarda tu mejor puntaje.

---

## Estructuras de datos utilizadas

| Estructura | Uso en el juego |
|------------|----------------|
| **Lista múltiplemente enlazada** | El mapa de la ciudad (cada nodo tiene 4 punteros: arriba, abajo, izquierda, derecha) |
| **Pila** (`std::stack`) | Las pistas recolectadas por el detective (LIFO — la última encontrada es la primera en usarse) |
| **Cola** (`std::queue`) | Las declaraciones de los testigos (FIFO — se interrogan en orden de encuentro) |
| **Tabla Hash** (`std::unordered_map`) | Los 8 sospechosos del caso (búsqueda por nombre en O(1) promedio) |
| **Árbol Binario de Búsqueda (ABB)** | El ranking histórico de detectives (implementado desde cero, ordenado por puntaje) |

---

## Estructura del proyecto

```
src/
├── main.cpp                  ← Punto de entrada (4 líneas)
├── Juego/                    ← Clase controladora del flujo del juego
├── Mapa/                     ← Lista múltiplemente enlazada (11×11)
├── Ubicacion/                ← Nodo de la lista (cada celda del mapa)
├── Detective/                ← Jugador + Pila de pistas
├── Pista/                    ← Los 4 tipos de pista
├── Testigo/                  ← Testigos del mapa
├── Sospechoso/               ← Entidad con nombre y atributos
├── TablaSospechosos/         ← Tabla Hash de sospechosos
├── ArbolPuntajes/            ← ABB para el ranking histórico
└── RankingJson/              ← Lectura/escritura de ranking.json
```

---

## Cómo correr el proyecto

1. Abrir el proyecto en **CLion** (o cualquier IDE con soporte CMake)
2. Compilar con CMake (C++17 requerido)
3. Ejecutar `ProyectoDetective`

```bash
# O desde terminal:
cd cmake-build-debug
./ProyectoDetective
```

---

## Integrantes

| Nombre | Rol |
|--------|-----|
| **Juan Felipe Perafán Ramos** | Desarrollador |
| **Isabella Garzón Salazar** | Desarrolladora |
| **Juan José López** | Desarrollador |

---


