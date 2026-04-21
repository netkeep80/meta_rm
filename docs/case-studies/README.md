# Case studies

This directory collects deep-analysis notes, reproduction logs, and investigation
artefacts produced while working on individual issues.

Layout:

```text
docs/case-studies/
  issue-<number>/
    README.md         # problem statement, root cause, final fix
    logs/             # CI logs, local reproduction logs
    experiments/      # scripts or minimal repros that were actually run
```

Only keep material that documents the *why* behind a change — throwaway scratch
work should live in `./experiments/` at the repository root and not be
committed.
