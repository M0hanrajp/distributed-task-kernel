# Contributing to Distributed Task Kernel (DTK)

We welcome contributions to the Distributed Task Kernel project\! Whether it's bug fixes, new features, documentation improvements, or anything else, your help is appreciated.

Please take a moment to review this document to understand how to contribute effectively.

## How to Contribute

1.  **Fork the Repository:** Start by forking the `dtk-project` repository to your GitHub account.
2.  **Clone Your Fork:** Clone your forked repository to your local machine:
    ```bash
    git clone https://github.com/M0hanrajp/distributed-task-kernel.git
    cd dtk-project
    ```
3.  **Create a New Branch:** Create a new branch for your feature or bug fix. Use a descriptive name (e.g., `feature/add-new-task-type`, `fix/scheduler-bug`):
    ```bash
    git checkout -b feature/your-feature-name
    ```
4.  **Make Your Changes:** Implement your changes in the new branch.
5.  **Test Your Changes:**
      * Ensure the project compiles without warnings or errors.
      * Run the application (`./a.out`) and manually test the functionality related to your changes.
      * Confirm that existing features still work as expected.
      * If adding a new feature, demonstrate its functionality.
6.  **Commit Your Changes:** Commit your changes with a clear and concise commit message mentioned below.
    ```bash
    git commit -m "feat: Add support for dynamic node scaling"
    ```
7.  **Push to Your Fork:** Push your new branch to your GitHub fork:
    ```bash
    git push origin feature/your-feature-name
    ```
8.  **Create a Pull Request (PR):**
      * Go to your forked repository on GitHub.
      * Click the "New Pull Request" button.
      * Ensure your branch is selected for the comparison.
      * Provide a clear title and a detailed description of your changes in the PR body. Reference any relevant issues.

## Code Style

  * **Readability:** Prioritize clear, readable code. Use meaningful variable and function names.
  * **Indentation:** Use consistent indentation (e.g., 4 spaces).
  * **Comments:** Add comments where necessary to explain complex logic or non-obvious parts of the code. Use Doxygen-style comments (`/** ... */` or `///`) for function documentation, as demonstrated in `dtk_kernel.hpp`.
  * **File Structure:** Maintain the existing file structure (`dtk_kernel.hpp`, `dtk_kernel.cpp`, `dtk_task_handler.cpp`, `main.cpp`).

## Reporting Bugs

If you find a bug, please open a new issue on the GitHub repository. When reporting a bug, please include:

  * **A clear and concise description of the bug.**
  * **Steps to reproduce** the behavior.
  * **Expected behavior.**
  * **Actual behavior.**
  * Any relevant **error messages or console output.**
  * Your **operating system and compiler version.**

## Suggesting Enhancements

We're open to new ideas\! If you have a suggestion for an enhancement or a new feature, please open a new issue on the GitHub repository. Describe your idea clearly and explain why you think it would be a valuable addition.

## Commit Message Guidelines

Please follow these guidelines for your commit messages:

  * **Type:** Start with a type tag (e.g., `feat`, `fix`, `docs`, `refactor`, `test`, `build`, `ci`).
      * `feat`: A new feature
      * `fix`: A bug fix
      * `docs`: Documentation only changes
      * `style`: Changes that do not affect the meaning of the code (white-space, formatting, missing semicolons, etc)
      * `refactor`: A code change that neither fixes a bug nor adds a feature
      * `perf`: A code change that improves performance
      * `test`: Adding missing tests or correcting existing tests
      * `build`: Changes that affect the build system or external dependencies
      * `ci`: Changes to CI configuration files and scripts
      * `chore`: Other changes that don't modify src or test files
  * **Subject:** A concise, imperative mood, lowercase subject line (max \~50 chars).
  * **Body (Optional):** A more detailed explanation if necessary. Wrap lines at 72 characters.

**Example:**

```
feat: Add dynamic node scaling
- Implemented logic to add/remove nodes during runtime.
- Updated scheduler to rebalance tasks when node count changes.
```

## Licensing

By contributing to DTK, you agree that your contributions will be licensed under the project's [LICENSE](https://github.com/M0hanrajp/distributed-task-kernel/blob/31e62b8bc8f4ba4fae8107da8e82239a6d1a6098/LICENSE) file.

-----
