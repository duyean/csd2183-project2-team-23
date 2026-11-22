# AI Use Log: Dijkstra vs Bellman-Ford

| Field | Details |
|---|---|
| **Date** | 22 November 2026 |
| **AI tool** | Claude (Anthropic), Claude Sonnet 5.5 via claude.ai |
| **Topic** | Shortest path algorithms (Dijkstra, Bellman-Ford) and priority queues in C++ |
| **Purpose** | Understanding concepts for a school project |
| **Nature of use** | Conceptual explanation only; no code was requested or generated |

---

## Exchange 1

### Prompt

> Cna you explain to me how dijkstra and bellman ford idffer? do not provide any psuedocode or any snippets of code. I want you to only research and provide me information

### Response

Both algorithms solve the single-source shortest path problem: given a starting node in a weighted graph, find the cheapest route to every other node. They differ in how they get there and in what kinds of graphs they can handle.

#### Core approach

**Dijkstra's algorithm** is greedy. It always picks the unvisited node with the smallest known distance from the source, declares that distance final, and then updates the distances of that node's neighbors. Once a node is settled, it is never revisited. This works because, with non-negative weights, any other path to that node would have to pass through nodes that are already farther away, so it can't be cheaper.

**Bellman-Ford** uses dynamic programming and brute-force persistence. It doesn't pick favorites. It repeatedly goes through every edge in the graph and checks whether going through that edge improves the distance to its endpoint. It repeats this for up to V-1 rounds (V being the number of vertices), because the longest possible shortest path without cycles has at most V-1 edges. After round *k*, all shortest paths using at most *k* edges are correct.

#### Negative edge weights

This is the biggest practical difference.

- **Dijkstra** fails with negative edges. Its assumption that a settled node can't be improved later breaks down, because a negative edge further along could make a "finalized" path cheaper. It may return wrong answers without warning.
- **Bellman-Ford** handles negative edges correctly. It can also detect negative cycles, meaning loops whose total weight is negative so that you could lower your path cost forever by going around. If an extra round after V-1 still improves a distance, a reachable negative cycle exists. Dijkstra has no such detection ability.

#### Time complexity

- **Dijkstra**: depends on the priority queue used. With a binary heap it runs in roughly O((V + E) log V). With a Fibonacci heap it improves to O(E + V log V).
- **Bellman-Ford**: O(V × E), since it processes every edge up to V-1 times. On dense graphs this can be dramatically slower.

So Dijkstra is much faster in practice, but only where it is valid.

#### Other differences

- **Early termination**: Dijkstra can stop as soon as the target node is settled if you only need one destination. Bellman-Ford can stop early only if a full round produces no changes.
- **Distributed settings**: Bellman-Ford is easier to run in a decentralized way because each node only needs information from its neighbors. This is why distance-vector routing protocols such as RIP are based on it, whereas link-state protocols like OSPF use Dijkstra, since each router has the full network map.
- **Implementation complexity**: Bellman-Ford is simpler conceptually and needs no priority queue. Dijkstra requires a good priority queue for its performance.

#### When to use which

Use Dijkstra when all edge weights are non-negative and speed matters, for example in road navigation or network routing with known costs. Use Bellman-Ford when negative weights are possible, when you need to detect negative cycles (as in currency arbitrage detection, where cycles in exchange rates can reveal profit opportunities), or when the decentralized nature of the problem fits.

If it would help, I can also cover related algorithms like Johnson's algorithm, which combines both to handle all-pairs shortest paths with negative edges.

---

## Exchange 2

### Prompt

> Since i need to keep track the data from the school projects i had to use a priority queue on c++, why is it so? can i not use a normal queue? why is c++ priority queue favoured in this scenario

### Response

The short version is that Dijkstra's correctness and speed both depend on always processing the closest unsettled node next, and a priority queue is the structure built to do that.

#### Why a normal queue doesn't work

A normal queue is first-in, first-out, so it hands back nodes in the order they were added, not in order of distance. That breaks Dijkstra's central rule. The algorithm only works because when it picks the node with the smallest known distance, no other path can beat it, so that distance is final. If you process nodes in arrival order instead, you may settle a node using a distance that is not yet the shortest, and later discover a cheaper route.

A plain queue behaves like breadth-first search, which finds the fewest edges rather than the lowest total weight. That is only correct when every edge has the same cost (an unweighted graph).

You could still get correct answers with a normal queue if you allow nodes to be re-added and reprocessed whenever their distance improves. That is essentially a queue-based variant of Bellman-Ford. It works, but you lose Dijkstra's efficiency, and in bad cases the running time degrades badly.

#### What the priority queue does for you

The priority queue holds the "frontier," meaning the nodes you have discovered but not yet finalized, each with its current best-known distance. It lets you:

- Pull out the smallest-distance node quickly, in logarithmic time.
- Insert newly discovered or improved nodes quickly, also in logarithmic time.

Without it, you would have to scan every unvisited node each time to find the minimum. That is a valid approach and gives roughly O(V²) overall, which is actually fine for small or very dense graphs, but it is slower on larger sparse graphs, where the priority queue version shines.

#### Why C++'s priority queue is the common choice

- **It's built in.** The standard library's priority queue is a ready-made binary heap, so you don't have to write your own heap, which is why it appears in most coursework and competitive programming.
- **It's efficient enough.** It gives the roughly O((V + E) log V) behavior mentioned earlier, which is the practical target for most projects.
- **It stores pairs naturally.** You can keep a distance and a node together as a single entry, which is exactly what the algorithm needs to track.

Two quirks you may have run into in your project:

- C++'s priority queue is a max-heap by default, returning the largest item first. Dijkstra needs the smallest, so people either reverse the comparison or store negated distances.
- It has no "decrease priority" operation. When a node's distance improves, the usual workaround is to insert a new, better entry and simply ignore the older, outdated one when it eventually surfaces. This is often called lazy deletion. The alternative is an ordered set, which allows removing the old entry and inserting the updated one, at slightly higher overhead and complexity.

So the priority queue isn't strictly mandatory, but it is the right tool because it directly supports "give me the closest unsettled node," which is the heart of the algorithm.

---

## Exchange 3

### Prompt

> create a md of this conversation so i can log it as AI use

### Response

This document.

---

## Notes for the log

- All content above is reproduced from the conversation. Prompts are shown exactly as typed, including typos.
- The AI was used for explanation of concepts only. No code was generated.
- Algorithm details (complexity figures, behavior with negative weights, C++ priority queue behavior) should be verified against course materials or an authoritative textbook such as *Introduction to Algorithms* (CLRS) before being relied on in submitted work.
