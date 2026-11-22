---
title: "Test Case Guide for Graph Algorithms - Google Gemini"
platform: gemini
source_url: "https://gemini.google.com/app/9e551b85b16aa58d?pli=1"
exported_at: "2026-11-22T07:30:41.836Z"
message_count: 40
tags:
  - ai-chat
  - gemini
---

# Test Case Guide for Graph Algorithms - Google Gemini

> **Platform:** GEMINI   
> **Exported on:** 11/22/2026, 3:30:41 PM  
> **Source:** [Original Conversation](https://gemini.google.com/app/9e551b85b16aa58d?pli=1)

---

### 👤 User

You said for this what kind of test cases are needed?  for this what kind of test cases are needed?


---

### 👤 User

You said for this what kind of test cases are needed?  for this what kind of test cases are needed?


---

### 👤 User

You said for this what kind of test cases are needed?  for this what kind of test cases are needed?


---

### 👤 User

You said for this what kind of test cases are needed?  for this what kind of test cases are needed?


---

### 👤 User

You said for this what kind of test cases are needed?  for this what kind of test cases are needed?


---

### 👤 User

You said for this what kind of test cases are needed?


---

### 🤖 Gemini

For Kestrel Isles, your team needs 10 `.txt` test files in `tests/` (in addition to `tests/example.txt`). Each file should be designed to expose a plausible defect in one or more of the required algorithms or edge cases in the specification.   project_2_kestrel_isles_assignment_brief.pdf+ 11. Rules & Constraints for Your 10 Team TestsFormat & Count: Create 10 `.txt` command files in `tests/` (excluding `tests/example.txt`). Start each file with a `#` comment stating the specific behavior or defect it targets.   project_2_kestrel_isles_assignment_brief.pdf+ 2Size & Limits: Keep each file to at most 10,000 commands and within the limits of Table 6 (at most 2×1052 \times 10^52×105 islands, quests, and events; 10610^6106 bridges, sea routes, and prerequisites; 100 queries; and at most 5,000 islands and 50,000 sea routes in any world queried with `stamina`).   project_2_kestrel_isles_assignment_brief.pdf+ 1Validity & Execution Limits: Every test must be valid input that runs cleanly with zero sanitizer errors on a correct implementation (`isles_debug`), completing within 120 seconds and producing at most 256 MB of output.   project_2_kestrel_isles_assignment_brief.pdf+ 12. High-Value Test Cases by Tool & Defect TypeMap Audit (`regions`, `hops`)Disconnected & Isolated Islands: Worlds with multiple components and completely isolated islands (`0` bridges) to verify region partitioning and `unreachable` responses.   project_2_kestrel_isles_assignment_brief.pdf+ 2Sea Routes Ignored by Bridge Queries: Adding sea routes (`R`) between separate bridge regions to verify that `regions` and `hops` only traverse bridges (`B`).   project_2_kestrel_isles_assignment_brief.pdf+ 2Same-Island Queries & Deep Chains: Querying `hops u u` (should be `0` hops) and constructing a long linear chain of islands to catch recursive DFS stack overflow (if DFS is used) or off-by-one hop counts.   project_2_kestrel_isles_assignment_brief.pdf+ 1Maintenance (`kruskal`, `prim`)Disconnected Forests: A bridge graph with multiple components (including isolated islands) to verify `kruskal` produces a spanning forest and `prim` restarts in every region rather than stopping after one component.   project_2_kestrel_isles_assignment_brief.pdf+ 1Equal-Weight Ties & Zero-Length Bridges: Bridges with tied lengths and `0` length to ensure sorting, tie-handling, and total length summation behave properly.   project_2_kestrel_isles_assignment_brief.pdf+ 1No Bridges / Single Island: Worlds with `N >= 1` and zero bridges, confirming `maintain` outputs total `0` and `0` bridges without out-of-bounds access.   project_2_kestrel_isles_assignment_brief.pdf+ 1Routes (`fastest`, `stamina`)Detour Beats Direct Edge (`fastest`): A direct bridge with higher length than a multi-bridge path, plus unreachable pairs and large weights up to 10910^9109 (checking for 32-bit integer overflow).   project_2_kestrel_isles_assignment_brief.pdf+ 2Negative Stamina & Costly-First Winner (`stamina`): Sea routes with negative costs (stamina restoration), including a case where a higher-cost first hop reaches a negative-cost route that beats a cheaper direct route.   project_2_kestrel_isles_assignment_brief.pdf+ 1Negative Cycles — Reachable vs. Unreachable from `s`:A negative cycle unreachable from source `s` (must not falsely report `exploit`).   project_2_kestrel_isles_assignment_brief.pdfA negative cycle reachable from `s` that does not lie on a path to `t` (must still report `exploit <cycle>`, and the reported cycle must be a valid negative cycle reachable from `s`).   project_2_kestrel_isles_assignment_brief.pdf+ 1Quest Schedule (`unlock`)Asymmetric Prerequisite Chains: Diamond or multi-path dependencies where a longer chain of prerequisites finishes later than a shorter chain, verifying both the earliest finish time and a valid prerequisite chain from a root quest to `q`.   project_2_kestrel_isles_assignment_brief.pdf+ 1Zero-Duration Quests & Unconnected Quests: Quests with `0` hours and multiple independent starting quests at time `0`.   project_2_kestrel_isles_assignment_brief.pdf+ 1Cycle Not Containing `q`: A prerequisite cycle among other quests while querying `unlock <q>` on a quest outside the cycle—the specification requires `unlock <q> cycle` whenever any quests contain a prerequisite cycle.   project_2_kestrel_isles_assignment_brief.pdfEvents (`most`, `best`)Half-Open Interval Boundaries `[start, finish)`: Events where one ends at time `t` and another starts at time `t` (compatible, no overlap) versus starting at `t - 1` (overlapping).   project_2_kestrel_isles_assignment_brief.pdf+ 1Cardinality vs. Reward Conflict: A set of many short, low-reward events alongside fewer long, high-reward events so `most` and `best` return different selections.   project_2_kestrel_isles_assignment_brief.pdf+ 1Large Timestamps & Rewards: Start/finish times up to 101210^{12}1012 and rewards up to 10910^9109 to catch integer overflow in time comparisons or reward sums.   project_2_kestrel_isles_assignment_brief.pdfMulti-World & Incremental StateWorld Resets (`N`) & Interleaved Queries: Multiple `N` commands in a single file that discard prior world state, mixed with incremental `B`/`R`/`Q`/`P`/`E` additions and repeated queries at different stages.   project_2_kestrel_isles_assignment_brief.pdf3. Cross-Checking & Sanitizer WorkflowWhile building and running your tests, use the built-in consistency checks and `isles_debug`:   project_2_kestrel_isles_assignment_brief.pdfRun with `isles_debug` so AddressSanitizer and UndefinedBehaviorSanitizer catch out-of-bounds reads/writes, uninitialized values, or signed overflows even if output looks right.   project_2_kestrel_isles_assignment_brief.pdf+ 1Cross-check `maintain kruskal` and `maintain prim` total lengths on the same bridge graph.   project_2_kestrel_isles_assignment_brief.pdfCross-check `fastest` against `bellman_ford` on a sea-route graph where every bridge `B u v w` is represented as two directed routes (`R u v w` and `R v u w`).   project_2_kestrel_isles_assignment_brief.pdfCross-check `most` count against `best` event count when all events have reward `1`.   project_2_kestrel_isles_assignment_brief.pdf


---

### 🤖 Gemini

Gemini said


---

### 🤖 Gemini

Gemini said


---

### 🤖 Gemini

For Kestrel Isles, your team needs 10 `.txt` test files in `tests/` (in addition to `tests/example.txt`). Each file should be designed to expose a plausible defect in one or more of the required algorithms or edge cases in the specification.   project_2_kestrel_isles_assignment_brief.pdf+ 11. Rules & Constraints for Your 10 Team TestsFormat & Count: Create 10 `.txt` command files in `tests/` (excluding `tests/example.txt`). Start each file with a `#` comment stating the specific behavior or defect it targets.   project_2_kestrel_isles_assignment_brief.pdf+ 2Size & Limits: Keep each file to at most 10,000 commands and within the limits of Table 6 (at most 2×1052 \times 10^52×105 islands, quests, and events; 10610^6106 bridges, sea routes, and prerequisites; 100 queries; and at most 5,000 islands and 50,000 sea routes in any world queried with `stamina`).   project_2_kestrel_isles_assignment_brief.pdf+ 1Validity & Execution Limits: Every test must be valid input that runs cleanly with zero sanitizer errors on a correct implementation (`isles_debug`), completing within 120 seconds and producing at most 256 MB of output.   project_2_kestrel_isles_assignment_brief.pdf+ 12. High-Value Test Cases by Tool & Defect TypeMap Audit (`regions`, `hops`)Disconnected & Isolated Islands: Worlds with multiple components and completely isolated islands (`0` bridges) to verify region partitioning and `unreachable` responses.   project_2_kestrel_isles_assignment_brief.pdf+ 2Sea Routes Ignored by Bridge Queries: Adding sea routes (`R`) between separate bridge regions to verify that `regions` and `hops` only traverse bridges (`B`).   project_2_kestrel_isles_assignment_brief.pdf+ 2Same-Island Queries & Deep Chains: Querying `hops u u` (should be `0` hops) and constructing a long linear chain of islands to catch recursive DFS stack overflow (if DFS is used) or off-by-one hop counts.   project_2_kestrel_isles_assignment_brief.pdf+ 1Maintenance (`kruskal`, `prim`)Disconnected Forests: A bridge graph with multiple components (including isolated islands) to verify `kruskal` produces a spanning forest and `prim` restarts in every region rather than stopping after one component.   project_2_kestrel_isles_assignment_brief.pdf+ 1Equal-Weight Ties & Zero-Length Bridges: Bridges with tied lengths and `0` length to ensure sorting, tie-handling, and total length summation behave properly.   project_2_kestrel_isles_assignment_brief.pdf+ 1No Bridges / Single Island: Worlds with `N >= 1` and zero bridges, confirming `maintain` outputs total `0` and `0` bridges without out-of-bounds access.   project_2_kestrel_isles_assignment_brief.pdf+ 1Routes (`fastest`, `stamina`)Detour Beats Direct Edge (`fastest`): A direct bridge with higher length than a multi-bridge path, plus unreachable pairs and large weights up to 10910^9109 (checking for 32-bit integer overflow).   project_2_kestrel_isles_assignment_brief.pdf+ 2Negative Stamina & Costly-First Winner (`stamina`): Sea routes with negative costs (stamina restoration), including a case where a higher-cost first hop reaches a negative-cost route that beats a cheaper direct route.   project_2_kestrel_isles_assignment_brief.pdf+ 1Negative Cycles — Reachable vs. Unreachable from `s`:A negative cycle unreachable from source `s` (must not falsely report `exploit`).   project_2_kestrel_isles_assignment_brief.pdfA negative cycle reachable from `s` that does not lie on a path to `t` (must still report `exploit <cycle>`, and the reported cycle must be a valid negative cycle reachable from `s`).   project_2_kestrel_isles_assignment_brief.pdf+ 1Quest Schedule (`unlock`)Asymmetric Prerequisite Chains: Diamond or multi-path dependencies where a longer chain of prerequisites finishes later than a shorter chain, verifying both the earliest finish time and a valid prerequisite chain from a root quest to `q`.   project_2_kestrel_isles_assignment_brief.pdf+ 1Zero-Duration Quests & Unconnected Quests: Quests with `0` hours and multiple independent starting quests at time `0`.   project_2_kestrel_isles_assignment_brief.pdf+ 1Cycle Not Containing `q`: A prerequisite cycle among other quests while querying `unlock <q>` on a quest outside the cycle—the specification requires `unlock <q> cycle` whenever any quests contain a prerequisite cycle.   project_2_kestrel_isles_assignment_brief.pdfEvents (`most`, `best`)Half-Open Interval Boundaries `[start, finish)`: Events where one ends at time `t` and another starts at time `t` (compatible, no overlap) versus starting at `t - 1` (overlapping).   project_2_kestrel_isles_assignment_brief.pdf+ 1Cardinality vs. Reward Conflict: A set of many short, low-reward events alongside fewer long, high-reward events so `most` and `best` return different selections.   project_2_kestrel_isles_assignment_brief.pdf+ 1Large Timestamps & Rewards: Start/finish times up to 101210^{12}1012 and rewards up to 10910^9109 to catch integer overflow in time comparisons or reward sums.   project_2_kestrel_isles_assignment_brief.pdfMulti-World & Incremental StateWorld Resets (`N`) & Interleaved Queries: Multiple `N` commands in a single file that discard prior world state, mixed with incremental `B`/`R`/`Q`/`P`/`E` additions and repeated queries at different stages.   project_2_kestrel_isles_assignment_brief.pdf3. Cross-Checking & Sanitizer WorkflowWhile building and running your tests, use the built-in consistency checks and `isles_debug`:   project_2_kestrel_isles_assignment_brief.pdfRun with `isles_debug` so AddressSanitizer and UndefinedBehaviorSanitizer catch out-of-bounds reads/writes, uninitialized values, or signed overflows even if output looks right.   project_2_kestrel_isles_assignment_brief.pdf+ 1Cross-check `maintain kruskal` and `maintain prim` total lengths on the same bridge graph.   project_2_kestrel_isles_assignment_brief.pdfCross-check `fastest` against `bellman_ford` on a sea-route graph where every bridge `B u v w` is represented as two directed routes (`R u v w` and `R v u w`).   project_2_kestrel_isles_assignment_brief.pdfCross-check `most` count against `best` event count when all events have reward `1`.   project_2_kestrel_isles_assignment_brief.pdf


---

### 👤 User

You said so it is best if I split the test cases out amongst all the different types (ie. kruskals, bst, erro…  so it is best if I split the test cases out amongst all the different types (ie. kruskals, bst, error cases like negative values or half open events)?


---

### 👤 User

You said so it is best if I split the test cases out amongst all the different types (ie. kruskals, bst, erro…  so it is best if I split the test cases out amongst all the different types (ie. kruskals, bst, error cases like negative values or half open events)?


---

### 👤 User

You said so it is best if I split the test cases out amongst all the different types (ie. kruskals, bst, erro…  so it is best if I split the test cases out amongst all the different types (ie. kruskals, bst, error cases like negative values or half open events)?


---

### 👤 User

You said so it is best if I split the test cases out amongst all the different types (ie. kruskals, bst, erro…  so it is best if I split the test cases out amongst all the different types (ie. kruskals, bst, error cases like negative values or half open events)?


---

### 👤 User

You said so it is best if I split the test cases out amongst all the different types (ie. kruskals, bst, erro…  so it is best if I split the test cases out amongst all the different types (ie. kruskals, bst, error cases like negative values or half open events)?


---

### 👤 User

You said so it is best if I split the test cases out amongst all the different types (ie. kruskals, bst, erro…


---

### 🤖 Gemini

Splitting your 10 test files across the different tools and edge-case behaviors is a great approach, though there are a few important adjustments based on the assignment specification:   project_2_kestrel_isles_assignment_brief.pdf1. Spread Across the Tools & Edge Cases (Not "BST")Cover the actual project algorithms: Instead of a BST (which isn't part of Kestrel Isles), focus on the algorithms and queries in the project: `regions`/`hops` (BFS/DFS), `maintain` (`kruskal`/`prim` + `DisjointSet`), `fastest` (`dijkstra`), `stamina` (`bellman_ford`), `unlock` (DAG DP/topological sort + cycle detection), and `most`/`best` (greedy + DP).   project_2_kestrel_isles_assignment_brief.pdf+ 1Multi-behavior files work too: Each `.txt` file can either target one focused defect category (like negative cycles in `stamina`, or prerequisite cycles in `unlock`) or combine closely related queries on a tricky graph—just make sure the `#` comment at the top clearly states the behavior or defect targeted.   project_2_kestrel_isles_assignment_brief.pdf2. Be Careful with "Negative Values" and "Error Cases"Negative values aren't automatically errors: Sea route (`R`) stamina costs can legitimately be negative (representing stamina restoration/tailwinds). Bridge lengths (`B`) and quest hours (`Q`) are non-negative (≥0\ge 0≥0), and event rewards (`E`) are positive (≥1\ge 1≥1).   project_2_kestrel_isles_assignment_brief.pdf+ 2Every test file must be valid input: The specification states that the driver checks commands, functions receive valid input, and every one of your test files must run with a correct implementation without error. Don't put malformed commands or out-of-range invalid inputs in your 10 `.txt` test files expecting them to fail—instead, put valid boundary/tricky cases that expose algorithmic or implementation defects (such as negative cycles unreachable from `s`, cycles outside `q` in `unlock`, or off-by-one overlap at `[start, finish)`).   project_2_kestrel_isles_assignment_brief.pdf+ 43. Half-Open Events `[start, finish)` Are Definitely Worth TestingTesting boundary timestamps where one event ends at `t` and another starts at `t` (compatible) versus starting before `t` (overlapping) is a classic spot for `<` vs. `<=` bugs in both `most_events` and `best_events`.   project_2_kestrel_isles_assignment_brief.pdf+ 1Which of the five tool areas (map audit, maintenance, routes, quest schedule, or events) have you already started implementing or have the least confidence in?


---

### 🤖 Gemini

Gemini said


---

### 🤖 Gemini

Gemini said


---

### 🤖 Gemini

Splitting your 10 test files across the different tools and edge-case behaviors is a great approach, though there are a few important adjustments based on the assignment specification:   project_2_kestrel_isles_assignment_brief.pdf1. Spread Across the Tools & Edge Cases (Not "BST")Cover the actual project algorithms: Instead of a BST (which isn't part of Kestrel Isles), focus on the algorithms and queries in the project: `regions`/`hops` (BFS/DFS), `maintain` (`kruskal`/`prim` + `DisjointSet`), `fastest` (`dijkstra`), `stamina` (`bellman_ford`), `unlock` (DAG DP/topological sort + cycle detection), and `most`/`best` (greedy + DP).   project_2_kestrel_isles_assignment_brief.pdf+ 1Multi-behavior files work too: Each `.txt` file can either target one focused defect category (like negative cycles in `stamina`, or prerequisite cycles in `unlock`) or combine closely related queries on a tricky graph—just make sure the `#` comment at the top clearly states the behavior or defect targeted.   project_2_kestrel_isles_assignment_brief.pdf2. Be Careful with "Negative Values" and "Error Cases"Negative values aren't automatically errors: Sea route (`R`) stamina costs can legitimately be negative (representing stamina restoration/tailwinds). Bridge lengths (`B`) and quest hours (`Q`) are non-negative (≥0\ge 0≥0), and event rewards (`E`) are positive (≥1\ge 1≥1).   project_2_kestrel_isles_assignment_brief.pdf+ 2Every test file must be valid input: The specification states that the driver checks commands, functions receive valid input, and every one of your test files must run with a correct implementation without error. Don't put malformed commands or out-of-range invalid inputs in your 10 `.txt` test files expecting them to fail—instead, put valid boundary/tricky cases that expose algorithmic or implementation defects (such as negative cycles unreachable from `s`, cycles outside `q` in `unlock`, or off-by-one overlap at `[start, finish)`).   project_2_kestrel_isles_assignment_brief.pdf+ 43. Half-Open Events `[start, finish)` Are Definitely Worth TestingTesting boundary timestamps where one event ends at `t` and another starts at `t` (compatible) versus starting before `t` (overlapping) is a classic spot for `<` vs. `<=` bugs in both `most_events` and `best_events`.   project_2_kestrel_isles_assignment_brief.pdf+ 1Which of the five tool areas (map audit, maintenance, routes, quest schedule, or events) have you already started implementing or have the least confidence in?


---

### 👤 User

You said I just want to know the scope of test cases I am to make  I just want to know the scope of test cases I am to make


---

### 👤 User

You said I just want to know the scope of test cases I am to make  I just want to know the scope of test cases I am to make


---

### 👤 User

You said I just want to know the scope of test cases I am to make  I just want to know the scope of test cases I am to make


---

### 👤 User

You said I just want to know the scope of test cases I am to make  I just want to know the scope of test cases I am to make


---

### 👤 User

You said I just want to know the scope of test cases I am to make  I just want to know the scope of test cases I am to make


---

### 👤 User

You said I just want to know the scope of test cases I am to make


---

### 🤖 Gemini

The scope of your team's custom test cases is:Quantity: 10 `.txt` command files placed in `tests/` (in addition to `tests/example.txt`).   project_2_kestrel_isles_assignment_brief.pdfPurpose: Each file must target a plausible defect or tricky behavior in the Kestrel Isles algorithms/queries, starting with a comment describing what it targets.   project_2_kestrel_isles_assignment_brief.pdf+ 1Coverage area: Valid inputs exercising the project's tools and edge cases—specifically map audit (`regions`, `hops`), maintenance (`maintain` with `kruskal` and `prim`), routes (`fastest` and `stamina`, including negative sea-route costs and negative cycles), quest schedule (`unlock`, DAG scheduling, and prerequisite cycles), events (`most` and `best`, including `[start, finish)` boundaries), and multi-world (`N`) state resets.   project_2_kestrel_isles_assignment_brief.pdf+ 2Size & runtime bounds: Each file must be valid input with at most 10,000 commands, stay within Table 6 limits (up to 2×1052\times 10^52×105 islands/quests/events, 10610^6106 bridges/sea routes/prerequisites, 100 queries, and at most 5,000 islands and 50,000 sea routes in any world with a `stamina` query), run cleanly without error on a correct implementation, and finish within 120 seconds and 256 MB of output.   project_2_kestrel_isles_assignment_brief.pdf+ 2


---

### 🤖 Gemini

Gemini said


---

### 🤖 Gemini

Gemini said


---

### 🤖 Gemini

The scope of your team's custom test cases is:Quantity: 10 `.txt` command files placed in `tests/` (in addition to `tests/example.txt`).   project_2_kestrel_isles_assignment_brief.pdfPurpose: Each file must target a plausible defect or tricky behavior in the Kestrel Isles algorithms/queries, starting with a comment describing what it targets.   project_2_kestrel_isles_assignment_brief.pdf+ 1Coverage area: Valid inputs exercising the project's tools and edge cases—specifically map audit (`regions`, `hops`), maintenance (`maintain` with `kruskal` and `prim`), routes (`fastest` and `stamina`, including negative sea-route costs and negative cycles), quest schedule (`unlock`, DAG scheduling, and prerequisite cycles), events (`most` and `best`, including `[start, finish)` boundaries), and multi-world (`N`) state resets.   project_2_kestrel_isles_assignment_brief.pdf+ 2Size & runtime bounds: Each file must be valid input with at most 10,000 commands, stay within Table 6 limits (up to 2×1052\times 10^52×105 islands/quests/events, 10610^6106 bridges/sea routes/prerequisites, 100 queries, and at most 5,000 islands and 50,000 sea routes in any world with a `stamina` query), run cleanly without error on a correct implementation, and finish within 120 seconds and 256 MB of output.   project_2_kestrel_isles_assignment_brief.pdf+ 2


---

### 👤 User

You said do these test cases need to be used in the same test or should I seperate test cases to test individ…  do these test cases need to be used in the same test or should I seperate test cases to test individual things (ie. one tests maintainece only in it's test case)


---

### 👤 User

You said do these test cases need to be used in the same test or should I seperate test cases to test individ…  do these test cases need to be used in the same test or should I seperate test cases to test individual things (ie. one tests maintainece only in it's test case)


---

### 👤 User

You said do these test cases need to be used in the same test or should I seperate test cases to test individ…  do these test cases need to be used in the same test or should I seperate test cases to test individual things (ie. one tests maintainece only in it's test case)


---

### 👤 User

You said do these test cases need to be used in the same test or should I seperate test cases to test individ…  do these test cases need to be used in the same test or should I seperate test cases to test individual things (ie. one tests maintainece only in it's test case)


---

### 👤 User

You said do these test cases need to be used in the same test or should I seperate test cases to test individ…  do these test cases need to be used in the same test or should I seperate test cases to test individual things (ie. one tests maintainece only in it's test case)


---

### 👤 User

You said do these test cases need to be used in the same test or should I seperate test cases to test individ…


---

### 🤖 Gemini

Separate test files—such as having a file focused on `maintain`—are generally better than cramming everything into one file.   project_2_kestrel_isles_assignment_brief.pdfClearer Defect Targeting: Each of the 10 `.txt` files in `tests/` must begin with a comment stating the specific behavior or defect it targets. Keeping a file focused on one area (like `maintain` across multiple disconnected components or Prim restarting per region) makes that intent and any resulting failure much easier to see.   project_2_kestrel_isles_assignment_brief.pdf+ 2Command & World Limits: Each file can contain up to 10,000 commands (and multiple `N` worlds if needed), within the limits of Table 6. You don't have to restrict a file to a single query if two queries interact on the same setup, though keeping each file focused on a clear set of behaviors is usually cleaner.   project_2_kestrel_isles_assignment_brief.pdf+ 2Total Count Is Fixed: Regardless of how you group them, your team submits ten test files of your own in `tests/` alongside `tests/example.txt`.   project_2_kestrel_isles_assignment_brief.pdf


---

### 🤖 Gemini

Gemini said


---

### 🤖 Gemini

Gemini said


---

### 🤖 Gemini

Separate test files—such as having a file focused on `maintain`—are generally better than cramming everything into one file.   project_2_kestrel_isles_assignment_brief.pdfClearer Defect Targeting: Each of the 10 `.txt` files in `tests/` must begin with a comment stating the specific behavior or defect it targets. Keeping a file focused on one area (like `maintain` across multiple disconnected components or Prim restarting per region) makes that intent and any resulting failure much easier to see.   project_2_kestrel_isles_assignment_brief.pdf+ 2Command & World Limits: Each file can contain up to 10,000 commands (and multiple `N` worlds if needed), within the limits of Table 6. You don't have to restrict a file to a single query if two queries interact on the same setup, though keeping each file focused on a clear set of behaviors is usually cleaner.   project_2_kestrel_isles_assignment_brief.pdf+ 2Total Count Is Fixed: Regardless of how you group them, your team submits ten test files of your own in `tests/` alongside `tests/example.txt`.   project_2_kestrel_isles_assignment_brief.pdf
