# Skjorheim

A C++ Text-Based RPG with Dynamic Narrative Branching.

## Overview
Skjorheim is a console-based role-playing game where the player's choices and character growth (via the "Grimoire" magic system) directly influence the narrative flow, dialogue, and endings. The project demonstrates the use of standard C++ data structures to create a reactive storytelling engine.

## Compilation & Run
To build the project, ensure you have a standard C++ compiler (like g++) installed.

```bash
g++ -Iinclude src/*.cpp -o skjorheim
./skjorheim
```

## Data Structures

### 1. Story Tree (Binary Tree)
The core narrative is structured as a Binary Tree where each `StoryNode` represents a scene.
- **Traversal**: Recursive traversal via `StoryTree::runNode`.
- **Branching**: Nodes have `left` and `right` children representing player choices.
- **Dynamic Routing**: Special "Router Nodes" (Event IDs 3 & 4) intercept the flow to redirect the player to different story branches (Silent Mortal, Touched, Voice Bearer) based on their Grimoire state.

### 2. Grimoire (Linked List)
The magic system uses a custom Singly Linked List (`GrimoireNode`) to manage "Words of Power".
- **CRUD**: Supports Create (Learn), Read (List), Update (Upgrade), and Delete (Forget) operations.
- **State Tracking**: The `Grimoire` class provides helper methods (`getWordCount`, `hasUpgradedWords`) to allow the story engine to query player power levels without exposing internal implementation details.

### 3. Finite State Machine (Battle System)
Combat is handled via a simple turn-based system triggered by flags in the `StoryNode`. It integrates with the Grimoire to allow usage of learned words in battle.

## Narrative Reactivity
The game features a unique "Reactivity" system where the story adapts to how the player interacts with the magic system:
- **0 Words**: The player is treated as a "Silent Mortal". NPCs are dismissive, and the Dragon denies them audience.
- **1 Word**: The player is "Touched by the Word". NPCs sense potential, and the Dragon acknowledges them.
- **Upgraded Words**: The player becomes a "Voice Bearer". The world reacts with awe/fear, and a special "True Balance" ending path becomes available.

## Code Organization
- `src/`: Implementation files (`.cpp`).
- `include/`: Header files (`.h`).
- `story_text/`: Externalized narration text files.
