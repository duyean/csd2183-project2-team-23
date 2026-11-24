https://chatgpt.com/share/6b052991-9b68-83ec-97ef-7ec40fdfa337

project_2_kestrel_isles_assignment_brief(2).pdf
PDF
can you explain to me this brief more in depth?
I've gone through the CSD2183 Project 2 — Kestrel Isles: Graph and Optimization Tools for a Game World assignment brief. I'll break it down section by section, explain what the algorithms actually do in the context of the game, and go into more detail about your assigned responsibilities.

One important thing about this assignment is that getting all the code to pass Gradescope is only a small portion of the overall project. Your team also needs to conduct experiments, write tests, investigate algorithms and explain your findings in a report.

1. Assignment overview
Submission deadline
28 Nov 2026
Saturday, 11:59 PM
Course grade weightage
12.5%
Marked out of 100
Programming language
C++23
g++ 15.2 on Linux
Report word limit
2,500
One report per team
Your team is essentially pretending to be a group of programmers working for a game studio developing an island-hopping adventure game.

You are not required to create an actual playable game. Instead, you are creating a C++ program that provides tools for the game's level designers.

The program helps the designers answer questions such as:

Which islands are connected?

What is the minimum number of bridges a player needs to cross?

Which bridges should the studio maintain to save money?

What is the shortest route between two islands?

Are there exploits that allow players to gain unlimited stamina?

How quickly can players complete quests?

Which events should players attend to maximize their rewards?

Each of these questions corresponds to an algorithm that you have learned in Data Structures. 
project_2_kestrel_isles_assignment_brief(2).pdf

2. Understanding the game world
There are four main elements in Kestrel Isles.

2.1 Bridges — Undirected graphs

Bridge network from the assignment's example world on pages 2 and 6.
Every circle represents an island, also known as a vertex or node.

Every line represents a bridge, also known as an edge.

The number on each bridge represents its length, also known as its weight.

Because bridges are undirected, if island 0 is connected to island 1, a player can travel in both directions.

This network is used for BFS, DFS, Disjoint Sets, Kruskal's, Prim's and Dijkstra's algorithms.

2.2 Sea routes — Directed graphs

Arrows represent the permitted direction of travel.
Unlike bridges, sea routes have a specific direction. A route from island 0 to island 1 does not automatically allow you to travel from island 1 to island 0.

Sea routes also have stamina costs.

A positive cost of 5 means the player loses 5 stamina, while a negative cost of −2 means the player gains 2 stamina.

This is why Bellman–Ford is required instead of standard Dijkstra's algorithm.

Bridges and sea routes are completely separate networks. You cannot combine them when calculating a route.

2.3 Quests — Dependency graphs

Certain quests require other quests to be completed first.

In this example, Quest 2 requires both Quest 0 and Quest 1 to finish. Quest 3 then requires Quest 2 to finish.

Multiple quests can run simultaneously, so you need to calculate the earliest possible completion time rather than simply adding every quest's duration together.

This uses topological sorting and dynamic programming.

2.4 Events — Interval scheduling
Events have a starting time, finishing time and reward. However, players cannot attend two overlapping events.


Events 0 and 2 provide the highest combined reward of 11 in the brief's example.
There are two different problems here: attending the largest number of events and earning the largest total reward. They are not necessarily the same thing. One uses a greedy algorithm, while the other uses dynamic programming.

All four of these systems make up the game world described in Sections 1 and 2. 
project_2_kestrel_isles_assignment_brief(2).pdf

3. What your team needs to implement
Your team is responsible for implementing the following functions.

Function

Required algorithm

Purpose

DisjointSet members

Union by rank + path compression

Manage connected sets

region_labels

BFS or DFS

Find connected islands

fewest_hops

BFS

Find minimum bridge crossings

kruskal

Kruskal's

Minimum spanning forest

prim

Prim's with binary heap

Minimum spanning forest

dijkstra

Dijkstra's with binary heap

Shortest walking distance

bellman_ford

Bellman–Ford

Minimum stamina cost and exploit detection

quest_schedule

Topological sort + DP

Earliest quest completion

most_events

Greedy earliest finish

Maximum number of events

best_events

Dynamic programming

Maximum event reward

You must use the specified algorithms. Even if another algorithm produces the correct answer, it does not necessarily satisfy the assignment requirements. 
project_2_kestrel_isles_assignment_brief(2).pdf

4. How the program works
The starter code already provides main.cpp, which reads commands from a text file.

For example:

N 6 0
B 0 1 4
B 1 2 3
B 0 2 6
B 3 4 2
regions
hops 0 2
The commands mean:

Command

Meaning

N 6 0

Create 6 islands and 0 quests

B 0 1 4

Connect islands 0 and 1 with a bridge of length 4

B 1 2 3

Connect islands 1 and 2 with a bridge of length 3

B 0 2 6

Connect islands 0 and 2 with a bridge of length 6

B 3 4 2

Connect islands 3 and 4 with a bridge of length 2

regions

Find all connected regions

hops 0 2

Find the fewest bridges from island 0 to island 2

The expected output is:

regions 3
0 1 2
3 4
5
hops 0 2 1
You do not need to implement the command-reading system or print this output yourself. The provided main.cpp handles those responsibilities by calling your functions.

Command file (.txt)

Contains the world and queries
main.cpp (provided)

Reads and validates the commands
Your algorithm functions

Calculate and return the results
main.cpp (provided)

Formats and prints the output
This also means that you should not add std::cout statements inside your functions to print the final answers. 
project_2_kestrel_isles_assignment_brief(2).pdf
 
project_2_kestrel_isles_assignment_brief(2).pdf

5. Your assigned responsibilities in depth
Based on the team responsibilities you shared previously, you are in charge of Map Audit & Disjoint-Set Foundation.

Your assigned work consists of three main functions, adjacency-list helpers if your team needs them, initial unit tests, and contributions to the Implementation and Testing sections of the report.

5.1 DisjointSet
Purpose: Efficiently determine whether two islands belong to the same connected set and merge sets together.

Initially, every island belongs to its own set.

Disjoint-set demonstration
Step 1 of 4

Initially, all islands are separate.

Parent array
0
0
1
1
2
2
3
3
4
4
5
5
The two fundamental operations are:

Find: Determines the representative (root) of a set. If two islands have the same root, they belong to the same set.

Union: Combines two different sets into one.

The brief specifically requires two optimizations.

Union by rank
When combining two sets, you attach the root with the smaller rank to the root with the larger rank. When the ranks are equal, one becomes the parent and its rank increases.

This prevents the trees from becoming unnecessarily deep.

Path compression
Imagine that your parent relationships look like this:

Before find(3)

3 parent links to root
After find(3)

1 parent link to root
Before path compression, finding the root of island 3 requires following:

3 → 2 → 1 → 0

After path compression, island 3 can directly access island 0.

This makes future operations much faster.

Why your teammate needs this
The teammate implementing Kruskal's algorithm will use your DisjointSet implementation to determine whether adding a bridge would create a cycle.

Therefore, your implementation is a foundation for another part of the assignment.

Time complexity
Operation

Time complexity

Initialization

O(V)

Find

O(α(V)) amortized

Union

O(α(V)) amortized

α(V) is the inverse Ackermann function, which grows extremely slowly. For practical input sizes, these operations are nearly constant time.

5.2 region_labels
Purpose: Find all connected components in the bridge network.

A connected component is a group of islands where every island can reach every other island in that group through bridges.

Finding connected regions
0/6 visited

All islands are initially unvisited.

Region labels (−1 means unvisited)
Island 0
-1
Island 1
-1
Island 2
-1
Island 3
-1
Island 4
-1
Island 5
-1
The general idea is to iterate through every island. Whenever you encounter an unvisited island, start a BFS or DFS and assign the same region label to every island you reach.

Once that traversal finishes, move on to the next unvisited island and assign a new region label.

For the example, the labels would be:

Island:  0  1  2  3  4  5
Region:  0  0  0  1  1  2
There are three regions in total.

Important: An isolated island is still considered a region.

The length of the bridges does not matter for this function. You are only concerned with whether the islands are connected.

Time complexity: O(V + E), using an adjacency list.

5.3 fewest_hops
Purpose: Determine the minimum number of bridges a player needs to cross to reach a destination.

You are required to use Breadth-First Search (BFS).


0 → 2: 1 hop

0 → 1 → 2: 2 hops

The answer for hops 0 2 is 1 because there is a direct bridge connecting islands 0 and 2.

BFS is suitable because it explores the graph level by level.

Example of BFS levels

The first time BFS reaches an island, it has found the minimum number of edges required to reach it.

Some important situations your implementation must handle:

Situation

Expected behavior

Source equals destination

0 hops

Directly connected islands

1 hop

Multiple possible paths

Choose fewest hops

Disconnected islands

Unreachable

Graph contains cycles

Avoid repeatedly visiting islands

Bridge has zero length

Still counts as 1 hop

Time complexity: O(V + E).

Both region_labels and fewest_hops ignore bridge lengths, as specified in the brief. 
project_2_kestrel_isles_assignment_brief(2).pdf

5.4 Adjacency-list implementation
Your team assigned you this responsibility:

Build the underlying graph representation helpers (adjacency lists) if shared across the team.

An adjacency list stores the neighbors of every vertex.

For the example bridge network, the adjacency list would look like this:

0

1
2
1

0
2
2

0
1
3

4
4

3
5

No neighbors
This allows BFS to quickly determine which islands it can visit next.

In C++, an unweighted adjacency list can be represented using:

std::vector<std::vector<int>>

Your teammates implementing Prim's and Dijkstra's algorithms may also need adjacency lists, but theirs need to store bridge weights as well.

Since you previously mentioned that your teammates have not asked to reuse your adjacency-list implementation, you do not necessarily need to create a separate shared helper. The brief allows shared helpers but does not make them compulsory. 
project_2_kestrel_isles_assignment_brief(2).pdf

6. Understanding your teammates' algorithms
Although these functions are not directly assigned to you, understanding them will help when writing the report and discussing the project with your team.

6.1 Kruskal's and Prim's algorithms
Both algorithms solve the minimum spanning forest problem.

Imagine that the studio wants to remove unnecessary bridges to reduce maintenance costs without disconnecting islands that were originally connected.

Original network
Minimum spanning forest

Total maintenance length
9
Bridges maintained
3
The bridge of length 6 can be removed because islands 0 and 2 can still reach each other through island 1.

The total maintenance length becomes:

4
+
3
+
2
=
9
4+3+2=9
Kruskal's sorts bridges by weight and adds them if they do not create a cycle, using your DisjointSet.

Prim's grows a tree by repeatedly choosing the cheapest bridge connecting the current tree to an unvisited island.

Both must produce the same minimum total length, although the actual bridges selected may differ.

6.2 Dijkstra's algorithm
Dijkstra's finds the shortest walking distance based on bridge lengths.

This is different from your fewest_hops function.


BFS — fewest hops
0 → 2

1 hop
Total length: 10
Dijkstra — shortest distance
0 → 1 → 2

Length 5
Total hops: 2
In this illustrative example, BFS chooses the direct bridge, but Dijkstra's chooses the two shorter bridges.

6.3 Bellman–Ford algorithm
Bellman–Ford calculates the cheapest voyage while allowing negative edge weights.

It also detects negative cycles.


Number of loops
1

Total stamina cost
-2
Stamina gained
+2
Every time the player travels around this cycle, they gain 2 stamina.

Therefore, the player can gain unlimited stamina by repeatedly travelling around the cycle.

The program must detect this exploit and return a valid negative cycle.

A particularly important requirement is that the negative cycle only needs to be reachable from the starting island. It does not need to affect the destination island.

6.4 Quest scheduling
Using the earlier quest example:


Quest completion timeline in hours
Earliest completion
8 hours
Critical chain
0 → 2 → 3
Quest 0 and Quest 1 can start together. Quest 2 must wait until both finish, so it starts at hour 3 instead of hour 5.

Quest 2 finishes at hour 7, and Quest 3 finishes at hour 8.

The algorithm must also detect cycles in the quest prerequisites, even if the cycle is unrelated to the quest being queried.

6.5 Most events and best events
The difference between these two functions is their objective.

most_events

best_events

Objective

Attend most events

Earn most rewards

Considers rewards

No

Yes

Algorithm

Greedy

Dynamic programming

Expected complexity

O(n log n)

O(n log n)

For example, imagine that there are three events:


Most events
Events 0 and 1

2 events
Total reward: 4
Best events
Event 2

10 reward
Total events: 1
The brief requires your team to investigate why greedy algorithms work for some problems but fail for others.

The detailed requirements for these algorithms are found in Sections 2.1 and 2.2. 
project_2_kestrel_isles_assignment_brief(2).pdf

7. Testing requirements — Section 4.2
Your team must create 10 additional test files, excluding the provided tests/example.txt.

Each test file must be a valid command file with the .txt extension, contain no more than 10,000 commands, and begin with a comment explaining what behavior or possible defect it targets.

The purpose is not simply to demonstrate that your code works on normal inputs. You should design tests that could expose mistakes in an incorrect implementation.

For your assigned portion, useful testing categories include:

Testing category

Possible defect being tested

Isolated islands

Not assigning isolated islands a region

Disconnected graphs

Incorrectly merging separate regions

Graphs containing cycles

Repeated visits or infinite loops

Source equals destination

Incorrect hop count

Different bridge lengths

BFS incorrectly considering weights

Long chains

Recursive traversal causing stack overflow

Repeated world creation

Retaining data from an earlier world

These are potential categories, not seven additional compulsory files for you personally. Your team needs ten files altogether, so coordinate the coverage with your teammates.

Running the tests
The normal build is:

make

The sanitizer-enabled build is:

make debug

On Windows, you can run a test using:

.\isles.exe tests/example.txt

The debug build is important because your program might produce the correct output while still having memory errors.

If make debug does not work on Windows, the brief allows you to rely on the Gradescope debug build. 
project_2_kestrel_isles_assignment_brief(2).pdf
 
project_2_kestrel_isles_assignment_brief(2).pdf

8. Experiments — Section 4.3
This is one of the more substantial parts of the assignment.

Your team must measure how algorithms perform as the input size increases, using the provided nanobench library.

You are not just measuring which algorithm is faster. You are supposed to explain why one algorithm performs better under particular conditions.

All experiments must run on the same machine, use at least four input sizes, and provide the same inputs to the algorithms being compared.

Experiment 1: Kruskal's versus Prim's
Your team needs to compare the two algorithms on three different graph structures.


Sparse

About 4n bridges

Dense

About n²/4 bridges

Grid

Neighbor connections
The required input sizes must include:

Graph type

Required size to include

Sparse

262,144 islands (2¹⁸)

Dense

4,096 islands (2¹²)

Grid

262,144 islands (2¹⁸)

You still need at least four sizes for each graph type.

Experiment 2: Disjoint-set optimizations
This experiment is particularly relevant to your assigned implementation.

Your team must compare four versions of DisjointSet.

Version

Union by rank

Path compression

Version 1



Version 2



Version 3



Version 4



The brief wants your team to copy DisjointSet into a benchmark as a class with another name.

You should not remove optimizations from your actual submitted DisjointSet implementation just to perform this experiment.

The benchmark needs to measure two things: Kruskal's running time using each version, and the average number of parent links followed during find.

For example, if one version's trees become deeper, its find operations may require following more parent links. Your measurements should help demonstrate whether the optimizations make a meaningful difference.

Your earlier team allocation did not explicitly mention who handles this benchmark, so I would check with your teammates about whether it is also assigned to you.

Experiment 3: Dijkstra's versus Bellman–Ford
Your team must compare the two algorithms on sparse random graphs and grid graphs with nonnegative weights.

Besides measuring execution times, your team needs to record how many rounds Bellman–Ford performs before it stops relaxing edges, explain how that relates to shortest paths, and investigate whether changing the order of edges affects the number of rounds.

What needs to be saved
Your repository must contain the benchmark source code, raw experimental results, analysis scripts and their dependencies.

The report must also state the compiler, operating system and processor used.

You should base your conclusions on actual measurements, rather than assuming the results will match theoretical expectations perfectly. 
project_2_kestrel_isles_assignment_brief(2).pdf

9. Greedy versus Optimal — Section 4.4
This section alone is worth 20 marks.

It is designed to test whether your team understands when an algorithm produces a guaranteed optimal answer.

There are five tasks in this section.

Task

What your team needs to do

Earliest finish

Prove why choosing the earliest-finishing event maximizes event count

Weighted events

Find counterexamples for three greedy selection rules

How far from optimal

Experimentally compare greedy rewards against optimal rewards

Dynamic programming

Explain the recurrences and subproblem graphs

When greedy breaks

Investigate why Dijkstra's can fail with a negative edge

Understanding the ratio experiments
Your team needs to compare three greedy approaches for weighted event scheduling:

Selecting the event with the largest reward.

Selecting the event with the earliest finishing time.

Selecting the event with the largest reward per unit of time.

Each greedy result is compared against the optimal result from best_events.

The ratio is calculated as:

Ratio
=
Greedy reward
Optimal reward
Ratio= 
Optimal reward
Greedy reward
​
 
Understanding the reward ratio
Optimal reward
100

Greedy reward
75

Optimal
100

Greedy
75

Reward ratio
0.75
Optimal reward achieved
75.0%
Illustrative values only, not experimental results.
A ratio of 1 means the greedy algorithm found an optimal answer.

A ratio of 0.75 means the greedy algorithm earned 75% of the optimal reward.

The brief requires at least 1,000 random instances at each of at least three input sizes, including a size of 1,000 events.

You also need two different reward models: one where rewards are independent of event duration, and one where rewards increase with duration.

This means at least 6,000 generated instances across the two models and three sizes, with the three greedy approaches compared against the optimal solution.

Your team then reports the mean and minimum ratios. 
project_2_kestrel_isles_assignment_brief(2).pdf

10. Report requirements — Section 5
Your team only needs to submit one report, using the provided Word template.

The entire report cannot exceed 2,500 words, excluding the title page and reference list.

Section

Suggested words

Implementation

400

Testing

250

Experiments

700

Greedy versus Optimal

450

AI Reflection

500

Conclusions

100

Contributions

100

Total

2,500

The individual section word counts are suggestions. The overall word limit is compulsory.

What you personally need to contribute
Implementation section
For your portion, you should explain your graph representation, why you chose BFS or DFS for region labeling, why BFS is appropriate for finding the fewest hops, how DisjointSet works, and the time complexities of your functions.

Remember that the suggested 400 words are for the entire team's Implementation section, not just yours.

Testing section
You should explain which defects your tests target, what edge cases you considered, and whether you discovered and fixed any actual defects.

Do not invent defects that you did not encounter.

Contributions section
Your team should accurately describe which functions, tests, experiments and report sections each member contributed to.

The report must also contain at least one table and one plot, with numbered captions, and must state its final word count. 
project_2_kestrel_isles_assignment_brief(2).pdf

11. Which files can you modify?
This is important because Gradescope restores certain files before compiling your submission.

File or folder

Editable?

Makefile

No

include/

No

src/main.cpp

No

Other files in src/

Yes

bench/

Yes

bench/nanobench/

No

data/

No

tests/

Yes

ai_logs/

Yes

report/

Yes

.githooks/

No

You are allowed to use standard C++ libraries such as std::vector, std::queue, std::priority_queue and std::sort.

Third-party algorithm libraries are not allowed.

You also do not need to commit generated executable files such as isles.exe. The submitted source code should be sufficient to build the program. 
project_2_kestrel_isles_assignment_brief(2).pdf

12. AI usage requirements — Section 6
You previously mentioned that your professor said the AI portion was an error. However, this version of the PDF still explicitly contains AI usage requirements and allocates 25 marks to them.

I cannot confirm whether your professor has officially overridden those requirements, so it would be worth clarifying exactly which parts no longer apply.

As written, the brief requires students to use browser-based AI chat, provide their own reasoning before asking for assistance, avoid adopting complete AI-generated functions, verify suggestions, and keep verbatim interaction logs.

The brief also specifies that non-merge Git commits must end with an AI declaration, such as:

AI: none
Or, if AI influenced the changes:

AI: 2026-11-24_chatgpt_bfs.md
The AI Reflection section should describe actual suggestions, how they were verified, and which ones were amended or rejected.

Until your professor confirms otherwise, I would not assume that all of Section 6 can be disregarded. 
project_2_kestrel_isles_assignment_brief(2).pdf

13. How the assignment is graded
Mark distribution
AI use

25 marks

Experiments

20 marks

Greedy versus Optimal

20 marks

Correctness

15 marks

Testing

10 marks

Implementation

10 marks

Total

100 marks

Notice that passing every instructor test only earns your team 15 out of 100 marks under the correctness component.

Experiments and Greedy versus Optimal together account for 40 marks.

Possible deductions
Issue

Deduction

Program exceeds one minute on an instructor command file

−20

Instructor cannot access GitHub repository

−10

Gradescope submission does not match assessed commit

−5

Report exceeds word limit or does not state word count

−5

All team members normally receive the same mark, but individual marks can be reduced if their contribution is negligible. 
project_2_kestrel_isles_assignment_brief(2).pdf
 
project_2_kestrel_isles_assignment_brief(2).pdf

14. Final submission requirements
There are three separate submission locations.

GitHub
All source code, tests, benchmarks, raw results, analysis scripts, counterexamples and AI logs must be on the default branch.

Gradescope
Submit the final GitHub repository through the GitHub integration and add all team members to the submission.

xSITe
One team member uploads the final Word report named team-<n>_report.docx.

Your team should complete the final GitHub push before submitting to Gradescope and uploading the report.

The time of the final xSITe upload determines whether the entire project is late. A GitHub push after the report upload will not be assessed unless the report is uploaded again. 
project_2_kestrel_isles_assignment_brief(2).pdf

15. Your personal assignment checklist
Based on the responsibilities you shared and our previous discussion about your implementation, this is what I would use to track your remaining work.

Personal progress tracker
0/19 completed
Programming


Implement all required DisjointSet members

Implement union by rank

Implement path compression

Implement region_labels

Implement fewest_hops

Verify all assigned functions pass instructor tests
Testing


Create tests for connected components

Create tests for minimum hop counts

Test isolated islands and disconnected graphs

Check sanitizer results through Gradescope
Team coordination


Confirm teammate can use DisjointSet for Kruskal's

Check whether a shared adjacency-list helper is needed

Confirm who is responsible for the DisjointSet benchmark
Report and submission


Contribute to the Implementation section

Contribute to the Testing section

Provide accurate time complexities

Document personal contributions

Clarify AI requirements with professor

Commit and push all required files
What I think you should focus on now
Since we have already worked on your map audit and DisjointSet implementation previously, I would prioritize verifying your functions, completing your tests and finishing your report contributions.

The main outstanding responsibility that I would clarify with your team is the DisjointSet experiment in Section 4.3. It is closely related to your implementation, but it was not explicitly listed in the team allocation you shared.

You do not need to implement Kruskal's, Prim's, Dijkstra's, Bellman–Ford or the event scheduling algorithms yourself unless your team changes its allocation.

Your main goal is to ensure that your three assigned components are correct, efficient, properly tested and clearly explained in the report.

can you explain to me my responsibilities that were given to me? 

Jerald: Map Audit & Disjoint-Set Foundation
Focus: Breadth-first/depth-first search, component labeling, and the disjoint-set data structure.

C++ Functions to Implement (src/):
DisjointSet::members (disjoint-set forest with union by rank and path compression).   
region_labels (connected components via BFS/DFS).   
fewest_hops (shortest bridge count via BFS).
   

Responsibilities:
Build the underlying graph representation helpers (adjacency lists) if shared across the team.   
Set up the initial unit tests for map components.
Contribute to the Implementation and Testing sections of the report regarding basic graph traversals.
Your assigned role, Map Audit & Disjoint-Set Foundation, is essentially responsible for building the foundation of the island network that some of your teammates' algorithms will rely on.

Compared to the entire assignment brief, your responsibilities can be broken down into four main areas:

Area

Your responsibility

Programming

Implement DisjointSet members, region_labels and fewest_hops

Graph representation

Create adjacency-list helpers if your teammates need them

Testing

Create initial tests for the island network

Documentation

Contribute to the Implementation and Testing sections of the report

I'll explain each responsibility, what it does, why it is needed and what you are expected to produce.

1. Implementing the C++ functions
These are your three main programming responsibilities. They correspond to Table 4 on page 5 of the assignment brief.

1.1 DisjointSet members
Difficulty: Moderate

Purpose: Keep track of which islands belong to the same group efficiently.

A DisjointSet, also known as a Union-Find data structure, allows you to group elements together and determine whether two elements belong to the same group.

Imagine that we have six islands.

DisjointSet demonstration
Step 1/4
0

Set 1
1

Set 2
2

Set 3
3

Set 4
4

Set 5
5

Set 6
Initially, every island belongs to its own set.

What does DisjointSet::members mean?
It does not necessarily mean that there is a function called members.

It means you are responsible for implementing the member functions of the DisjointSet class declared inside:

include/disjoint_set.h
You will need to look at the header to determine the exact function names and return types.

Typically, a DisjointSet contains the following operations:

Operation

Responsibility

Constructor

Initialize every island into its own set

Find

Find the representative of an island's set

Union

Merge two different sets

Other declared members

Follow the requirements in the provided header

The assignment specifically requires you to implement two optimizations.

Optimization 1: Union by rank
When combining two trees, you want to prevent the resulting tree from becoming unnecessarily tall.


The root of the lower-ranked tree becomes a child of the higher-ranked root.
When merging two sets:

If the first root has a higher rank, attach the second root to it.

If the second root has a higher rank, attach the first root to it.

If both ranks are equal, choose one root and increase its rank by one.

If both elements already belong to the same set, no merge is necessary.

This keeps the trees relatively shallow.

Optimization 2: Path compression
Path compression makes future searches for the root faster.

Suppose you want to find the representative of island 3.

Before path compression

Follow 3 parent links
After path compression

Follow 1 parent link
Originally, the algorithm must travel through:

3 → 2 → 1 → 0
After path compression, the nodes encountered during the search point directly to the root.

3 → 0
This means future searches require less work.

Why is this your responsibility?
Your teammate implementing Kruskal's algorithm needs your DisjointSet to detect whether adding a bridge would create a cycle.


DisjointSet tells Kruskal's that islands 0 and 2 already belong to the same set.
You do not need to implement Kruskal's yourself. You only need to provide a working DisjointSet that your teammate can use.

Expected time complexity: O(α(V)) amortized per find or union operation, which is practically near constant time.

1.2 region_labels
Difficulty: Easy to moderate

Purpose: Identify how many separate groups of connected islands exist in the world.

This function is used when the program receives the regions command.

Consider the example from the assignment brief.


There are six islands but only three connected regions.

Island

Region label

0

0

1

0

2

0

3

1

4

1

5

2

Conceptually, your function produces labels like:

{0, 0, 0, 1, 1, 2}

The provided driver then handles displaying the results.

regions 3
0 1 2
3 4
5
How does the algorithm work?
You are allowed to use either BFS or DFS.

I would recommend using BFS because you also need BFS for your fewest_hops function. This allows you to become familiar with one traversal algorithm instead of implementing two different ones.

Region labeling walkthrough
Step 1/7

Initialize all islands as unvisited.

Island 0
-1

Island 1
-1

Island 2
-1

Island 3
-1

Island 4
-1

Island 5
-1

−1 represents an unvisited island in this example.
What you need to be careful of
Your function must handle isolated islands, disconnected graphs and graphs containing cycles. It must also ignore sea routes and bridge lengths.

If you choose DFS, avoid recursive DFS because the assignment allows graphs containing up to 200,000 islands, which could cause a stack overflow.

Expected time complexity: O(V + E), where V is the number of islands and E is the number of bridges.

1.3 fewest_hops
Difficulty: Easy to moderate

Purpose: Find the minimum number of bridges required to travel from one island to another.

This function is called when the program receives a command such as:

hops 0 3
This means that the player wants to travel from island 0 to island 3 while crossing as few bridges as possible.

You are required to use BFS.

Understanding BFS
BFS explores the nearest islands first before moving further away.

BFS traversal demonstration
Distance 0

Start at island 0. Its distance is 0.

Island 0
0

Island 1
−

Island 2
−

Island 3
−

Island 4
−

Island 5
−

You will generally need a queue to store islands waiting to be explored and a distance array to keep track of the number of bridges crossed.

Fewest hops versus shortest distance
This is a very important distinction.


Your BFS function
0 → 2

1 hop
Bridge length: 100
Teammate's Dijkstra function
0 → 1 → 2

Length 5
2 hops
Even though the direct bridge is much longer, your function should return 1 because it only counts the number of bridges.

Your teammate's Dijkstra implementation considers the actual bridge lengths.

Expected behavior
Scenario

Expected result

Source and destination are the same

0

Islands are directly connected

1

Two bridges are needed

2

Destination cannot be reached

Unreachable

Bridge length is zero

Still counts as one hop

Check map_audit.h for the exact return value required when the destination is unreachable.

Expected time complexity: O(V + E).

2. Building adjacency-list helpers
Difficulty: Easy

Your second responsibility is to build the underlying graph representation if your teammates need to share it.

An adjacency list is simply a way of storing which islands are connected to which.

Suppose you receive these bridges:

B 0 1 4
B 1 2 3
B 0 2 6
B 3 4 2
You could represent their connections like this:

0

1
2
1

0
2
2

0
1
3

4
4

3
5

Empty
For your BFS functions, you only need to know the neighboring islands.

An appropriate data structure could be:

std::vector<std::vector<int>>

However, your teammates implementing Prim's and Dijkstra's algorithms may also need the bridge lengths.

A weighted adjacency list could use:

std::vector<std::vector<std::pair<int, int>>>

Each pair could represent a neighboring island and the bridge length.

Why use an adjacency list?
Adjacency list

Adjacency matrix

O(V + E) memory

O(V²) memory

Efficient neighbor traversal

Must scan a row for neighbors

Suitable for large sparse graphs

Expensive for large graphs

Since the assignment allows up to 200,000 islands, an adjacency matrix would consume an impractical amount of memory.

Is this compulsory?
Not necessarily. Your team allocation specifically says "if shared across the team."

If your teammates already have their own graph representations, you can simply create a helper for your own two BFS functions.

If they want to share your helper, coordinate the data structure and function interface with them first.

3. Setting up the initial unit tests
Difficulty: Easy to moderate

Your responsibility does not end after implementing the functions. You must also test them.

The assignment requires ten additional command-file tests across the entire team.

You are responsible for setting up the initial tests related to the map components.

Here are the testing scenarios I would recommend you consider.

Test scenario

What you are checking

Single island

One region and zero hops to itself

Multiple isolated islands

Every island receives its own region

Fully connected network

All islands belong to one region

Disconnected network

Separate regions and unreachable destinations

Network containing cycles

BFS does not repeatedly visit islands

Different bridge lengths

BFS ignores bridge weights

Sea routes without bridges

Sea routes do not connect bridge regions

World reset

Old connections are not retained

You do not necessarily need eight separate test files. Multiple related scenarios can be grouped together.

How the testing process works
Create a test command file
Calculate the expected answer manually
Run your C++ program
Compare actual and expected results
Fix defects and test again
The test files should be placed inside the tests/ directory.

Each file must begin with a comment explaining what potential defect it targets.

For DisjointSet, you can also coordinate with your teammate implementing Kruskal's to verify that sets merge correctly and that redundant unions do not cause incorrect behavior.

Remember that the assignment's command-file tests and direct C++ unit tests are slightly different. The ten required team tests are command files.

4. Contributing to the report
You have been assigned two sections of the report.

4.1 Implementation section
Your team has a suggested budget of 400 words for this entire section.

For your contribution, you should be able to explain the following:

Topic

What to explain

Graph representation

Why you used adjacency lists

DisjointSet

How union by rank and path compression work

Region labeling

How BFS or DFS identifies connected components

Fewest hops

Why BFS guarantees the minimum bridge count

Efficiency

Time complexities of your algorithms

Your contribution to the complexity table would look conceptually like this:

Function

Time complexity

Auxiliary space

DisjointSet initialization

O(V)

O(V)

DisjointSet find

O(α(V)) amortized

Implementation-dependent

DisjointSet union

O(α(V)) amortized

Implementation-dependent

region_labels

O(V + E)

O(V + E)

fewest_hops

O(V + E)

O(V + E)

The space complexities for the BFS functions above include building an adjacency list.

You do not need to explain your teammates' algorithms in your own contribution.

4.2 Testing section
Your team has a suggested budget of 250 words for this entire section.

For your contribution, you should explain what scenarios you tested, why those scenarios are important, what defects they could expose, and what actual defects you found and fixed.

For example, if you discovered that your BFS was not handling isolated islands correctly, you would explain how your test exposed that problem and how you resolved it.

You should write this section after completing your testing so that it accurately reflects your actual implementation.

5. Are you responsible for the experiments?
There is one additional task in the assignment brief that is closely related to your role, but is not explicitly mentioned in your team allocation.

On page 10, Section 4.3 requires a benchmark comparing four versions of DisjointSet.

Version

Union by rank

Path compression

Both optimizations



Rank only



Compression only



Neither optimization



The experiment measures the running time of Kruskal's algorithm using each version, as well as the average number of parent links followed by find.

You should confirm with your team whether you are responsible for this experiment. It is relevant to your implementation, but your assigned responsibilities do not explicitly include it.

6. Which files should you work on?
Based on the assignment brief, these are the files and directories relevant to your role.

Project repository

include/ Read only

disjoint_set.h

map_audit.h

src/

DisjointSet implementation file

Map audit implementation file

main.cpp Do not edit

tests/

Your map component tests

report/

Report contributions

The exact .cpp filenames should be confirmed from your starter code.

Do not modify the provided headers or main.cpp, because Gradescope restores the original versions of these files before compiling.

7. Recommended order of completing your work
I would recommend completing your responsibilities in this order.

Your personal checklist
0/10 completed

Read the provided headers

Understand the required functions, parameters and return types.

Implement DisjointSet

Your teammate needs this for Kruskal's algorithm.

Create the adjacency-list helper

Prepare the graph representation for BFS.

Implement region_labels

Learn how to traverse and label connected components.

Implement fewest_hops

Reuse your understanding of BFS to calculate distances.

Create map component tests

Cover normal cases and edge cases.

Run Gradescope tests

Check correctness and sanitizer errors.

Confirm benchmark responsibilities

Check who is handling the DisjointSet experiment.

Complete report contributions

Document your implementation and actual testing results.

Commit and push your work

Ensure your contributions are included in the repository.
8. What you are not responsible for
Based on the team allocation you provided, the following are not directly assigned to you.

Function

Your responsibility?

DisjointSet members

Yes

region_labels

Yes

fewest_hops

Yes

kruskal

No, but it depends on your DisjointSet

prim

No

dijkstra

No

bellman_ford

No

quest_schedule

No

most_events

No

best_events

No

DisjointSet benchmark

Confirm with team

You also do not need to implement the command parser, output formatting, an actual game, a graphical interface or the entire report by yourself.

To put everything simply, your role answers three fundamental questions:

DisjointSet: Do these two islands belong to the same set?

region_labels: Which islands belong to each connected region?

fewest_hops: How many bridges must a player cross to reach another island?

Once you have these three components working, your tests completed and your report contributions written, you will have fulfilled the main responsibilities assigned to you.

I would start by looking at disjoint_set.h and map_audit.h, as those two headers will tell you exactly what functions you need to implement.

a
Would you like me to walk you through implementing your assigned functions, starting with DisjointSet, step by step?





