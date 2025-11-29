# Project To-Do List (Master Overview)

1. **Environment & System Setup**
   - ~~Install Fedora + Sway environment.~~
   - ~~Install development tools (gcc/clang, make/CMake, git, terminal tools).~~
   - ~~Configure Git globally (name, email, editor).~~
   - ~~Generate SSH keys and confirm GitHub authentication works.~~
   - Set up ClangFormat + integrate it into Vim.
   - Set up a Git pre-commit hook for formatting + reminder to update documentation.
   - Install optional documentation tools (Doxygen).
   - ~~Install your Sway utilities (notification daemon, Waybar, network management, etc.).~~

2. **Repository Setup & Project Management**
   - ~~Create the project GitHub repo.~~
   - Add README + description + goals + scope.
   - Create a roadmap/milestones document.
   - ~~Add your initial to-do list to the repo.~~
   - Enable issue tracking and labels for tasks.
   - Set up branching strategy:
     - main for stable releases
     - testing branch
     - feature branches for every new change.

3. **Requirements & Planning Phase**
   - Requirements gathering template: define inputs, outputs, behaviors.
   - Use cases, user stories, and constraints.
   - Feasibility analysis:
     - technical feasibility
     - operational
     - economic
   - Define long-term goals + initial scope.

4. **System Architecture Design**
   - Decide communication between backend and frontend (IPC, sockets, etc.).
   - Define backend component layout.
   - Define terminal interface responsibilities.
   - Decide data model abstraction.
   - Plan error handling + logging + concurrency rules.
   - Design interaction between modules.

5. **Build Minimal Front-End (First)**
   - (Not the final, just the test harness)
   - Terminal interface that:
     - accepts commands
     - sends queries
     - prints returns
   - This is just to test the backend.

6. **Backend Database Development**
   - Implement core data structures for storage.
   - Implement relationships between entities.
   - Implement indexing/search system.
   - Implement I/O + binary storage format.
   - Implement querying API between front and backend.
   - Implement serialization/deserialization.

7. **Integration: Backend ↔ Frontend**
   - Wire up communication interface.
   - Verify queries work end-to-end.
   - Handle error conditions and invalid queries.
   - Begin basic testing of workflow.

8. **Testing & Quality**
   - Unit tests for every function.
   - Integration tests for communication between modules.
   - End-to-end tests for full queries.
   - Automated testing whenever possible.
   - User tests or feedback.

9. **Documentation & Maintenance**
   - Maintain README and code documentation.
   - Add developer instructions.
   - Keep docs updated whenever pushing changes.
   - Consider Doxygen for deeper documentation.
   - Keep issue list + milestones updated.

10. **Launch Initial Version**
    - Package first version.
    - Confirm functionality matches scope:
      - two data types (cars + people)
      - linking + searching
    - Verify everything works.

11. **Future Enhancements**
    - Add homes, addresses, new entity types.
    - Richer front-end interface.
    - More advanced querying and visualization.
    - Extend storage models.
