# Skjorheim

Skjorheim is a C++ text-based RPG that invites players into a mythic, Skyrim-inspired world. The game features a branching narrative, turn-based combat, and a "Words of Power" magic system managed via a custom Grimoire data structure.

## Overview

In Skjorheim, you play as a protagonist awakening in a cold, hostile land. Your journey involves making critical choices that affect your alignment (Order vs. Chaos vs. Balance), engaging in combat with creatures like wolves and dragons, and discovering ancient Words of Power.

## Key Features

*   **Branching Storyline:** A narrative engine built on a binary tree structure (`StoryTree`), allowing for impactful choices and multiple endings.
*   **Turn-Based Combat:** A strategic battle system (`BattleSystem`) where you manage Health Points (HP) and choose between Light Attacks, Heavy Attacks, and Magic (Words of Power).
*   **Grimoire System:** A custom linked-list implementation (`Grimoire`) to manage learned spells (CRUD operations: Create/Learn, Read/View, Update/Upgrade, Delete/Forget).
*   **Atmospheric Presentation:** Custom text rendering with typing effects and color coding to enhance immersion.

## Data Structures

The project demonstrates the use of several fundamental data structures:

1.  **Binary Tree (`StoryTree` & `StoryNode`)**:
    *   Used to model the narrative flow.
    *   Each node represents a story event or decision point.
    *   Traversals (Left/Right) correspond to player choices.
    *   Special nodes handle complex routing (e.g., checking Grimoire state) without expanding the tree structure.

2.  **Singly Linked List (`Grimoire` & `GrimoireNode`)**:
    *   Manages the player's collection of "Words of Power".
    *   Supports dynamic addition (learning words), removal (forgetting words), and modification (upgrading words).
    *   Chosen over `std::vector` to demonstrate manual memory management and pointer manipulation.

3.  **Finite State Machine (Implicit)**:
    *   The `BattleSystem` operates as a simple turn-based state machine (Player Turn <-> Enemy Turn), handling win/loss conditions.

## Grimoire & Story Impact

The **Grimoire** is not just a list of spells; it directly influences the narrative:
*   **0 Words:** The protagonist is "silent" and may be perceived as weak or prey by powerful entities like the Dragon.
*   **1 Word:** The protagonist has potential, unlocking specific dialogue options and recognition from NPCs.
*   **Upgraded/Multiple Words:** The protagonist is a true "Voice Bearer," commanding respect (or fear) and altering the endings significantly.

Certain story paths and endings are only accessible based on your mastery of the Voice.

## How to Build and Run

### Requirements
*   A C++ compiler (GCC recommended).
*   Standard C++ libraries.

### Compilation
Navigate to the project root directory and run:

```bash
g++ -Iinclude src/*.cpp -o skjorheim
```

### Running the Game
```bash
./skjorheim
```

## Controls
*   **Input:** Type the number corresponding to your choice and press **Enter**.
*   **Typing Effect:** Press **Enter** during text display to skip the typing animation.
