#!/usr/bin/env bash
# Enforce per-file line limits to keep files reviewable by humans and AI.
#
# See docs/BEST-PRACTICES in the hive-mind project:
#   - Code files: 1500 lines max
#   - Markdown files: 2500 lines max
#
# Fails the CI job (and prints a GitHub Actions annotation) on any violation.

set -euo pipefail

CODE_LIMIT=${CODE_LIMIT:-1500}
DOC_LIMIT=${DOC_LIMIT:-2500}

code_glob=(
    'include'
    'tests'
    'examples'
    'scripts'
)

violations=0

count_lines() {
    # wc -l counts newlines; add one if the file doesn't end with a newline.
    local file=$1
    local lines
    lines=$(wc -l <"$file")
    if [ -s "$file" ] && [ "$(tail -c1 "$file" | wc -l)" -eq 0 ]; then
        lines=$((lines + 1))
    fi
    printf '%s\n' "$lines"
}

check_file() {
    local file=$1 limit=$2
    local lines
    lines=$(count_lines "$file")
    if [ "$lines" -gt "$limit" ]; then
        echo "::error file=${file}::${file} has ${lines} lines (limit: ${limit})"
        violations=$((violations + 1))
    fi
}

for dir in "${code_glob[@]}"; do
    if [ -d "$dir" ]; then
        while IFS= read -r -d '' file; do
            check_file "$file" "$CODE_LIMIT"
        done < <(find "$dir" \
            \( -name '*.hpp' -o -name '*.h' -o -name '*.cpp' -o -name '*.cc' -o -name '*.sh' \) \
            -type f -print0)
    fi
done

while IFS= read -r -d '' file; do
    check_file "$file" "$DOC_LIMIT"
done < <(find . \
    -type d \( -name build -o -name .git -o -name node_modules \) -prune \
    -o -type f -name '*.md' -print0)

if [ "$violations" -gt 0 ]; then
    echo "::error::${violations} file(s) exceed the configured line limits."
    exit 1
fi

echo "All files within line limits (code <= ${CODE_LIMIT}, docs <= ${DOC_LIMIT})."
