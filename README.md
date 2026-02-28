# Discrete Mathematics Automation System (DMAS)

## Overview
The **Discrete Mathematics Automation System (DMSA)** is a high-performance C++ engine designed to automate the definition, analysis, and verification of discrete mathematical structures. It provides a rigorous computational environment for checking mathematical properties, conducting logical proofs, and executing complex graph algorithms on user-defined data.

Unlike standard calculators, DMSA treats **Sets, Relations, Functions, and Graphs** as first-class objects that can be manipulated, composed, and analyzed for deep mathematical properties (e.g., injectivity, transitivity, Eulerian paths).

---

## Core Architectures & Features

### 1. Set Theory Engine
The foundation of the system, supporting heterogeneous data types (**Integers, Strings, Characters**).

**Operations:**
- Union (`∪`)
- Intersection (`∩`)
- Difference (`-`)
- Symmetric Difference (`Δ`)

**Advanced Features:**
- Cartesian Products (`A × B`)
- Power Sets (`𝒫(A)`)

**Logic:**
- Subset (`⊆`) and Proper Subset (`⊂`) verification

---

### 2. Predicate Logic & Quantifiers
A logic validator that supports complex query expressions.

**Quantifiers:**
- Universal (`∀`): "For all elements `x`, is `P(x)` true?"
- Existential (`∃`): "Does there exist an `x` such that `P(x)` is true?"
- Unique Existential (`∃!`): "Is there exactly one `x` such that `P(x)` is true?"

**Advanced:**
- Nested Quantifiers, e.g., `∀x ∃y, P(x,y)`

---

### 3. Relation Algebra
Analyzes binary relations on sets.

**Properties:**
- Reflexivity
- Symmetry
- Anti-symmetry
- Transitivity

**Classification:**
- Equivalence Relations
- Partial Orders

**Closures:**
- Transitive (Warshall's Algorithm)
- Symmetric
- Reflexive

---

### 4. Function Theory
Models mappings between **Domain** and **Codomain** sets.

**Classification:**
- Injective (One-to-One)
- Surjective (Onto)
- Bijective (Invertible)

**Operations:**
- Function Composition (`f ∘ g`)
- Inverse Calculation (`f⁻¹`)

**Type Safety:**
- Supports cross-type mappings (e.g., Strings → Integers)

---

### 5. Graph Theory Algorithms
Supports both **Directed** and **Undirected** graphs with weighted edges.

**Connectivity:**
- Counts connected components

**Pathfinding:**
- Dijkstra’s Algorithm (shortest path)

**Spanning Trees:**
- Prim’s Algorithm for Minimum Spanning Trees (MST)
- Supports constraints (e.g., critical nodes inclusion)

**Traversals:**
- Eulerian Circuits/Paths
- Hamiltonian Cycles

---

## Technical Design & Modern C++ Usage

- **C++20** for type and memory safety.
- **`std::variant` & `std::visit`** for heterogeneous sets.
- **Templates & Concepts** to enforce type constraints (`template <Comparable T>`).
- **Smart Pointers (`std::unique_ptr`)** to prevent memory leaks.
- **Lambdas & Functional Programming** for flexible logic validation.

---


## Input File Syntax

### Defining Sets
```
#DEFINE_SET: Primes
Type: INT
Data: {2, 3, 5, 7, 11}
```

#DEFINE_GRAPH: NetworkMap
```
Type: STRING
Vertices: Cities
Directed: TRUE
Edges: [(London, Paris, 50), (Paris, Berlin, 120)]
```

---
# Standard Set Operations
OPERATION: UNION, SetA, SetB

# Graph Algorithms
OPERATION: SHORTEST_PATH, NetworkMap, London, Berlin
OPERATION: MST, NetworkMap, London

# Logic Checks
OPERATION: QUANTIFIER_CHECK, UNIVERSAL, Primes, IS_POSITIVE


