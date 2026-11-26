# Accessibility Statement

The **Universal Polyglot Layer (UPL+)** project is committed to ensuring that our documentation, developer toolchain, compiler diagnostics, and public community spaces are accessible to everyone, including developers with disabilities, visual impairments, or motor function limitations.

We believe that systems programming and language runtime engineering should be barrier-free and inclusive.

---

## 1. Priorities & Commitments

Our core accessibility priorities focus on:

* **Terminal & CLI Accessibility:**
  * Clean, standard output streams (`stdout` and `stderr`) that can be parsed effectively by terminal screen readers (NVDA, JAWS, Orca).
  * Avoidance of hardcoded ANSI escape sequences that break non-standard terminal emulators.
  * Clear text-based compiler diagnostic messages with explicit file, line, and column coordinates.
* **Documentation & Web Assets:**
  * Adherence to **WCAG 2.1 Level AA** standards across all documentation and markdown files.
  * Semantic Markdown structure (hierarchical headings, ordered/unordered lists, descriptive link text).
  * High contrast text, colorblind-safe diagram palettes, and text alternatives (`alt` text) for architectural diagrams and screenshots.
* **Keyboard-First Navigation:**
  * All development scripts, sample runners, and IDE extensions (`upl-vscode`) are operable using keyboard navigation alone.

---

## 2. Contributor Expectations

When contributing to UPL+ documentation, code, or tools, please keep the following guidelines in mind:

* **Alternative Text:** Always provide descriptive `alt` text for images and diagrams embedded in documentation.
* **Readable Code Blocks:** Include explicit language identifiers on code fences (e.g., ````c````, ````upl````, ````bash````) to assist syntax highlighters and assistive technology.
* **Color as Information:** Never rely solely on color to convey diagnostic status, test results, or severity levels (always include descriptive text symbols like `[PASS]`, `[FAIL]`, `[ERROR]`, `[WARN]`).

---

## 3. Reporting Accessibility Issues

If you encounter an accessibility barrier when using the UPL+ compiler toolchain, reading our documentation, or participating in our community:

1. **GitHub Issue:** Open a GitHub Issue using the **Bug Report** template and include the label `accessibility` (or note "Accessibility Barrier" in the title).
2. **Details to Include:**
   * The specific documentation page, CLI command, or tool component.
   * The assistive technology or environment used (e.g., screen reader version, OS high-contrast mode).
   * The observed barrier and the expected accessible experience.
3. **Email Contact:** You can also contact project maintainers directly at **accessibility@cyberenigma.io**.

---

## 4. Severity & Triage

Accessibility issues are prioritized during triage:

* **High Severity:** Critical barriers that completely block users of assistive technologies from building, reading core documentation, or running tests.
* **Medium Severity:** Barriers that cause significant friction or confusing screen reader output, but have an available workaround.
* **Low Severity:** Minor contrast, formatting, or semantic improvements.
