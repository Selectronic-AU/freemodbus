# Code Review Guidelines

## Review Goal

Review only for concrete correctness, security, and regression issues. If you cannot point to a specific failure mode, do not comment.

### Code Review Outcomes

Every review must use exactly one of these two overall outcomes:
1. "Changes recommended" only when the current changes contain at least one **unresolved** high-severity issue that must be fixed before merge.
2. "Approved" in every other case.

Never use any other overall review outcome.
Never leave a neutral, comment-only, informational, or uncertain review.
Never leave or summarize the overall review as "Needs a closer look".
If you cannot prove a blocking issue from the diff, approve.
Uncertainty is not a valid review state.

If a segment of code requires manual verification or further human testing, keep the overall review as "Approved" unless you found a concrete blocking defect. Put those follow-ups under a markdown header titled "### Items for Human Verification".

## Required Behavior

- Review only the changes since the last review.
- Comment only on new or still-unresolved issues in those changes.
- Comment only on a concrete bug, security issue, data loss risk, build break, or clearly demonstrated performance regression.
- If the issue is not reproducible from the diff alone, do not comment.
- If the code is correct and meets the requirements, approve it instead of looking for cosmetic issues.
- Keep the rest of the review guidance focused on concrete defects, not style or preference.
- Every review decision must briefly state the failing behavior, impact, and why it matters.
- If a review is "Approved", a brief justification should also be provided, even if it is simply "No **unresolved** issues found".
- Do not convert uncertainty, questions, or desired manual testing into a non-approval review state.

## Do Not Comment On

Do not comment on these unless they hide a real defect:

- Style or formatting: indentation, whitespace, bracket placement, semicolons, or similar CI-linted issues.
- Naming preferences: alternative names, except for misleading, broken, or obviously incorrect names.
- Refactoring preferences: alternative patterns, restructuring, or cleanup suggestions without a correctness, security, or severe performance reason.
- Missing comments or docstrings, unless the existing documentation is wrong or misleading.
- Pedantic accuracy: typo-level issues or wording nits that do not change the meaning.

## Scope Rules

- Review only the files and behavior changed by the pull request.
- Ignore all files under `demo/*` and `examples/*` unless the PR explicitly introduces them.
- Ignore bundled third-party code and vendored dependencies inside demo/example directories, including FreeRTOS, lwIP, and similar libraries. Ignore any issues that might result from them being out of date or not matching the latest upstream versions.

## Build System Rules

- Treat CMake as the primary build system. Ignore other build systems, such as Makefiles, unless the pull request explicitly changes them.
- Comment on build system changes only when they break correctness, reproducibility, or the build pipeline.
- Do not comment on build system style or personal preference.
- Treat CI, local developer workflow, and reproducibility as part of correctness for build changes.