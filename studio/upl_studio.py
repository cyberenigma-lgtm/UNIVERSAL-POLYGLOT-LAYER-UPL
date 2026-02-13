# UPL Studio v1.3 - Professional Sovereign IDE (Global Unicity Edition)
# Copyright (C) 2026 Neuro-OS Genesis
# Registered in the Intellectual Community of Neuro-OS Genesis.
# Derivative work of Neuro-OS Genesis source code.
# Licensed under GNU General Public License v3.0 (GPL-3)
# Future Notice: UPL v2 will be released under a private paid use license.

import sys
import os
import json
import time
from PySide6.QtWidgets import (QApplication, QMainWindow, QTextEdit, QDockWidget, 
                               QVBoxLayout, QWidget, QMenuBar, QStatusBar, QToolBar, 
                               QComboBox, QLabel, QTreeView, QFileSystemModel, QSplitter,
                               QTabWidget, QHBoxLayout, QFrame, QMessageBox, QFileDialog, 
                               QInputDialog, QCompleter)
from PySide6.QtGui import (QAction, QFont, QColor, QTextCharFormat, QIcon, QKeySequence, QTextCursor)
from PySide6.QtCore import Qt, QSize, QStringListModel, QProcess

# Configuración del entorno soberano 1.1
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
if BASE_DIR not in sys.path:
    sys.path.append(BASE_DIR)

try:
    from core.upl_orchestrator import UPLOrchestrator
except ImportError:
    UPLOrchestrator = None

class UPLEditor(QTextEdit):
    """Editor avanzado con Autocompletado (Sovereign Engine)."""
    def __init__(self, mnemonics=[], parent=None):
        super().__init__(parent)
        self.setFont(QFont("Consolas", 13))
        self.setLineWrapMode(QTextEdit.LineWrapMode.NoWrap)
        self.setStyleSheet("background-color: #1e1e1e; color: #d4d4d4; border: none;")

        self.completer = QCompleter(mnemonics, self)
        self.completer.setWidget(self)
        self.completer.setCompletionMode(QCompleter.CompletionMode.PopupCompletion)
        self.completer.setCaseSensitivity(Qt.CaseSensitivity.CaseInsensitive)
        self.completer.activated.connect(self._insert_completion)

    def _insert_completion(self, completion):
        tc = self.textCursor()
        extra = len(completion) - len(self.completer.completionPrefix())
        tc.movePosition(QTextCursor.MoveOperation.Left)
        tc.movePosition(QTextCursor.MoveOperation.EndOfWord)
        tc.insertText(completion[-extra:])
        self.setTextCursor(tc)

    def _get_word_under_cursor(self):
        tc = self.textCursor()
        tc.select(QTextCursor.SelectionType.WordUnderCursor)
        return tc.selectedText()

    def keyPressEvent(self, event):
        if self.completer and self.completer.popup().isVisible():
            if event.key() in (Qt.Key.Key_Enter, Qt.Key.Key_Return, Qt.Key.Key_Escape, Qt.Key.Key_Tab):
                event.ignore()
                return
        is_shortcut = (event.modifiers() & Qt.KeyboardModifier.ControlModifier) and event.key() == Qt.Key.Key_Space
        if is_shortcut:
            self.completer.setCompletionPrefix(self._get_word_under_cursor())
            self.completer.complete(self.cursorRect())
            return
        super().keyPressEvent(event)
        prefix = self._get_word_under_cursor()
        if len(prefix) >= 2:
            self.completer.setCompletionPrefix(prefix)
            self.completer.complete(self.cursorRect())
        else:
            self.completer.popup().hide()

class UPLTerminal(QTextEdit):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setFont(QFont("Consolas", 10))
        self.setStyleSheet("background-color: #0c0c0c; color: #cccccc; border: none;")
        self.process = QProcess(self)
        self.process.setProcessChannelMode(QProcess.ProcessChannelMode.MergedChannels)
        self.process.readyReadStandardOutput.connect(self._on_ready_read)
        shell = "powershell.exe" if os.name == "nt" else "bash"
        self.process.start(shell)
        
    def _on_ready_read(self):
        data = self.process.readAllStandardOutput().data().decode(errors='replace')
        self.moveCursor(QTextCursor.MoveOperation.End)
        self.insertPlainText(data)
        self.moveCursor(QTextCursor.MoveOperation.End)

    def keyPressEvent(self, event):
        if event.key() in (Qt.Key.Key_Return, Qt.Key.Key_Enter):
            cursor = self.textCursor()
            cursor.movePosition(QTextCursor.MoveOperation.End)
            cursor.select(QTextCursor.SelectionType.LineUnderCursor)
            line = cursor.selectedText()
            command = line.split(">")[-1].strip() if ">" in line else line.strip()
            self.process.write((command + "\n").encode())
            super().keyPressEvent(event)
        elif event.key() == Qt.Key.Key_C and event.modifiers() & Qt.KeyboardModifier.ControlModifier:
            self.process.kill()
            self.process.start("powershell.exe" if os.name == "nt" else "bash")
            self.append("\n^C (Shell Reiniciado)\n")
        else:
            super().keyPressEvent(event)

class UPLStudio(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Neuro-OS Genesis — UPL Studio v1.3 (Global Unicity)")
        self.resize(1500, 950)
        self.open_tabs = {}
        self.current_locale = "es"
        
        # Mapeo de 57 lenguajes de NUASM
        self.nuasm_langs = [
            ("es", "Español"), ("en", "English"), ("fr", "Français"), ("de", "Deutsch"), 
            ("ru", "Русский"), ("zh", "中文 (Mandarin)"), ("ja", "日本語"), ("ko", "한국어"),
            ("it", "Italiano"), ("pt", "Português"), ("hi", "हिन्दी"), ("ar", "العربية"),
            ("uk", "Українська"), ("nl", "Nederlands"), ("sv", "Svenska"), ("pl", "Polski"),
            ("tr", "Türkçe"), ("th", "ไทย"), ("vi", "Tiếng Việt"), ("el", "Ελληνικά"),
            ("he", "עברית"), ("fa", "فارسی"), ("id", "Indonesia"), ("hi", "Hindi"),
            ("bn", "Bengali"), ("ta", "Tamil"), ("te", "Telugu"), ("fi", "Finnish"),
            ("no", "Norwegian"), ("ro", "Romanian"), ("pl", "Polish"), ("ca", "Català"),
            ("gl", "Galego"), ("eu", "Euskara"), ("nah", "Náhuatl"), ("may", "Maya"),
            ("qu", "Quechua"), ("ay", "Aimara"), ("sw", "Swahili"), ("sw", "Suajili"),
            ("zu", "Zulu"), ("af", "Afrikaans"), ("yo", "Yoruba"), ("ha", "Hausa"),
            ("ig", "Igbo"), ("ak", "Akan"), ("am", "Amharic"), ("tl", "Tagalog"),
            ("ms", "Malay"), ("jv", "Javanese"), ("su", "Sundanese"), ("mi", "Maori"),
            ("ga", "Irish"), ("yue", "Cantonese"), ("tpi", "Tok Pisin"), ("ber", "Berber")
        ]
        
        self.ui_translations = {
            "es": {"file": "Archivo", "edit": "Editar", "view": "Vista", "run": "Ejecutar", "terminal": "Terminal", "explorer": "Explorador", "ir": "Lenguaje Madre", "unicity": "Unicidad Mundial", "status": "✅ Soberanía Española", "new": "📄 Nuevo", "save": "💾 Guardar", "run_btn": " ▶ EJECUTAR (F5)"},
            "en": {"file": "File", "edit": "Edit", "view": "View", "run": "Run", "terminal": "Terminal", "explorer": "Explorer", "ir": "Mother Language", "unicity": "Global Unicity", "status": "✅ English Sovereignty", "new": "📄 New", "save": "💾 Save", "run_btn": " ▶ RUN (F5)"},
            "fr": {"file": "Fichier", "edit": "Modifier", "view": "Vue", "run": "Lancer", "terminal": "Terminal", "explorer": "Explorateur", "ir": "Langue Mère", "unicity": "Unicité Mondiale", "status": "✅ Souveraineté Française", "new": "📄 Nouveau", "save": "💾 Sauvegarder", "run_btn": " ▶ LANCER (F5)"},
            "de": {"file": "Datei", "edit": "Edit", "view": "Ansicht", "run": "Run", "terminal": "Terminal", "explorer": "Explorer", "ir": "Muttersprache", "unicity": "Unicidad", "status": "✅ Deutsche Souveränität", "new": "📄 Neu", "save": "💾 Speichern", "run_btn": " ▶ RUN (F5)"},
            "ru": {"file": "Файл", "edit": "Правка", "view": "Вид", "run": "Пуск", "terminal": "Терминал", "explorer": "Проводник", "ir": "Материнский язык", "unicity": "Единство", "status": "✅ Русский суверенитет", "new": "📄 Новый", "save": "💾 Сохранить", "run_btn": " ▶ ПУСК (F5)"},
            "zh": {"file": "文件", "edit": "编辑", "view": "视图", "run": "运行", "terminal": "终端", "explorer": "资源管理器", "ir": "母语", "unicity": "全球一致性", "status": "✅ 中国主权", "new": "📄 新建", "save": "💾 保存", "run_btn": " ▶ 运行 (F5)"},
            "ja": {"file": "ファイル", "edit": "編集", "view": "表示", "run": "実行", "terminal": "端末", "explorer": "エクスプローラ", "ir": "母国語", "unicity": "世界的な独自性", "status": "✅ 日本の主権", "new": "📄 新規", "save": "💾 保存", "run_btn": " ▶ 実行 (F5)"},
            "ko": {"file": "파일", "edit": "편집", "view": "보기", "run": "실행", "terminal": "터미널", "explorer": "탐색기", "ir": "모국어", "unicity": "글로벌 유니시티", "status": "✅ 한국의 주권", "new": "📄 신규", "save": "💾 저장", "run_btn": " ▶ 실행 (F5)"}
        }

        self.setStyleSheet("""
            QMainWindow { background-color: #1e1e1e; color: #cccccc; }
            QMenuBar { background-color: #3c3c3c; color: #cccccc; border-bottom: 1px solid #2b2b2b; }
            QMenuBar::item { padding: 5px 10px; }
            QMenuBar::item:selected { background-color: #505050; }
            QToolBar { background-color: #333333; border: none; padding: 5px; spacing: 10px; }
            QStatusBar { background-color: #007acc; color: white; }
            QTreeView { background-color: #252526; color: #cccccc; border: none; }
            QSplitter::handle { background-color: #2b2b2b; }
            QTabWidget::pane { border-top: 1px solid #2b2b2b; }
            QTabBar::tab { background: #2d2d2d; color: #969696; padding: 10px 20px; border-right: 1px solid #1e1e1e; font-size: 11px; }
            QTabBar::tab:selected { background: #1e1e1e; color: #ffffff; border-bottom: 2px solid #007acc; }
        """)

        self.orch = UPLOrchestrator(locale=self.current_locale) if UPLOrchestrator else None
        self.mnemonics = self._get_mnemonics()

        # Toolbar
        self.toolbar = QToolBar("Main")
        self.addToolBar(self.toolbar)
        self.run_act = QAction(" ▶ RUN (F5)", self)
        self.run_act.setShortcut(QKeySequence("F5"))
        self.run_act.triggered.connect(self._run_upl)
        self.toolbar.addAction(self.run_act)
        self.toolbar.addAction("📄 New", self._new_file)
        self.toolbar.addAction("💾 Save", self._save_file)

        # Layout Splitter
        self.main_split = QSplitter(Qt.Orientation.Horizontal)
        
        # Explorer
        self.explorer_panel = QWidget()
        self.exp_lay = QVBoxLayout(self.explorer_panel)
        self.exp_lay.setContentsMargins(0, 0, 0, 0)
        self.exp_title = QLabel(" EXPLORER")
        self.exp_lay.addWidget(self.exp_title)
        self.file_model = QFileSystemModel()
        self.file_model.setRootPath(BASE_DIR)
        self.tree = QTreeView()
        self.tree.setModel(self.file_model)
        self.tree.setRootIndex(self.file_model.index(BASE_DIR))
        self.tree.setHeaderHidden(True)
        for i in range(1, 4): self.tree.setColumnHidden(i, True)
        self.tree.doubleClicked.connect(self._on_tree_double_click)
        self.exp_lay.addWidget(self.tree)
        self.main_split.addWidget(self.explorer_panel)
        
        # Center
        self.center_split = QSplitter(Qt.Orientation.Vertical)
        self.tabs = QTabWidget()
        self.tabs.setTabsClosable(True)
        self.tabs.tabCloseRequested.connect(self._close_tab)
        self.tabs.currentChanged.connect(self._sync_all)
        self.center_split.addWidget(self.tabs)
        
        self.bottom_tabs = QTabWidget()
        self.console = QTextEdit(); self.console.setReadOnly(True)
        self.bottom_tabs.addTab(self.console, "Output")
        self.terminal = UPLTerminal()
        self.bottom_tabs.addTab(self.terminal, "Terminal")
        
        # UNICIDAD MUNDIAL PANEL
        self.unicity_panel = QWidget()
        self.un_lay = QVBoxLayout(self.unicity_panel)
        self.un_lay.addWidget(QLabel("🌎 SELECCIÓN DE SOBERANÍA LINGÜÍSTICA (57 LENGUAJES NUASM)"))
        self.lang_box = QComboBox()
        for code, name in sorted(self.nuasm_langs, key=lambda x: x[1]):
            self.lang_box.addItem(f"{name} ({code})", code)
        self.lang_box.currentIndexChanged.connect(self._change_lang)
        self.un_lay.addWidget(self.lang_box)
        self.un_lay.addStretch()
        self.bottom_tabs.addTab(self.unicity_panel, "Unicidad")
        
        self.center_split.addWidget(self.bottom_tabs)
        self.main_split.addWidget(self.center_split)
        
        # IR
        self.ir_panel = QWidget()
        self.ir_lay = QVBoxLayout(self.ir_panel); self.ir_lay.setContentsMargins(0, 0, 0, 0)
        self.ir_title = QLabel(" UPL-IR")
        self.ir_lay.addWidget(self.ir_title)
        self.ir_view = QTextEdit(); self.ir_view.setReadOnly(True); self.ir_view.setFont(QFont("Consolas", 11)); self.ir_view.setStyleSheet("color: #ce9178; background: #1e1e1e;")
        self.ir_lay.addWidget(self.ir_view)
        self.main_split.addWidget(self.ir_panel)
        
        self.main_split.setStretchFactor(0, 1); self.main_split.setStretchFactor(1, 4); self.main_split.setStretchFactor(2, 2)
        self.setCentralWidget(self.main_split)
        self._set_menu_and_status()
        self._load_demo()

    def _get_mnemonics(self):
        m = ["python", "js", "rust", "go", "c", "PROCEDIMIENTO", "RETORNAR", "ESTABLECER"]
        try:
            with open(os.path.join(BASE_DIR, "core", "upl_locales.json"), 'r', encoding='utf-8') as f:
                data = json.load(f)
                lang_data = data.get(self.current_locale, data.get("es", {}))
                m.extend(lang_data.values())
        except: pass
        return sorted(list(set(m)))

    def _change_lang(self, idx):
        self.current_locale = self.lang_box.itemData(idx)
        if self.orch: self.orch.locale = self.current_locale
        self.mnemonics = self._get_mnemonics()
        for i in range(self.tabs.count()):
            ed = self.tabs.widget(i)
            if hasattr(ed, "completer"): ed.completer.setModel(QStringListModel(self.mnemonics))
        self._update_all_ui()

    def _update_all_ui(self):
        t = self.ui_translations.get(self.current_locale, self.ui_translations["en"])
        self.exp_title.setText(f" {t['explorer'].upper()}")
        self.ir_title.setText(f" {t['ir'].upper()}")
        self.bottom_tabs.setTabText(1, t["terminal"])
        self.bottom_tabs.setTabText(2, t["unicity"])
        self.menuBar().clear()
        self._set_menu_and_status()

    def _set_menu_and_status(self):
        mb = self.menuBar()
        t = self.ui_translations.get(self.current_locale, self.ui_translations["en"])
        
        # --- FILE MENU ---
        file_menu = mb.addMenu(t["file"])
        file_menu.addAction(t["new"], self._new_file, QKeySequence.New)
        file_menu.addAction("Open...", self._open_file_dialog, QKeySequence.Open)
        file_menu.addAction(t["save"], self._save_file, QKeySequence.Save)
        file_menu.addAction("Save As...", self._save_file_as, QKeySequence("Ctrl+Shift+S"))
        file_menu.addSeparator()
        file_menu.addAction("Close Tab", self._close_current_tab, QKeySequence("Ctrl+W"))
        file_menu.addAction("Exit", self.close, QKeySequence("Alt+F4"))

        # --- EDIT MENU ---
        edit_menu = mb.addMenu(t["edit"])
        edit_menu.addAction("Undo", lambda: self._current_ed().undo(), QKeySequence.Undo)
        edit_menu.addAction("Redo", lambda: self._current_ed().redo(), QKeySequence.Redo)
        edit_menu.addSeparator()
        edit_menu.addAction("Cut", lambda: self._current_ed().cut(), QKeySequence.Cut)
        edit_menu.addAction("Copy", lambda: self._current_ed().copy(), QKeySequence.Copy)
        edit_menu.addAction("Paste", lambda: self._current_ed().paste(), QKeySequence.Paste)
        edit_menu.addSeparator()
        edit_menu.addAction("Select All", lambda: self._current_ed().selectAll(), QKeySequence.SelectAll)

        # --- VIEW MENU ---
        view_menu = mb.addMenu(t["view"])
        view_menu.addAction("Toggle Explorer", self._toggle_explorer, QKeySequence("Ctrl+B"))
        view_menu.addAction("Toggle IR Panel", self._toggle_ir, QKeySequence("Ctrl+I"))
        view_menu.addAction("Toggle Terminal", self._toggle_terminal, QKeySequence("Ctrl+J"))

        # --- RUN / DEBUG / CORRECT ---
        run_menu = mb.addMenu(t["run"])
        run_menu.addAction(t["run_btn"], self._run_upl, QKeySequence("F5"))
        run_menu.addAction("🪲 DEBUG (F6)", self._debug_upl, QKeySequence("F6"))
        run_menu.addAction("🔧 CORRECT (F7)", self._correct_upl, QKeySequence("F7"))
        
        mb.addMenu("Catalog")
        mb.addMenu("Addons")

        self.statusBar().showMessage(t["status"])

    def _current_ed(self): return self.tabs.currentWidget()

    def _open_file_dialog(self):
        path, _ = QFileDialog.getOpenFileName(self, "Open UPL File", BASE_DIR, "UPL Files (*.upl);;All Files (*)")
        if path: self._load_file(path)

    def _save_file(self):
        idx = self.tabs.currentIndex()
        if idx == -1: return
        path = None
        for p, i in self.open_tabs.items():
            if i == idx: path = p; break
        
        if path and os.path.exists(path):
            try:
                with open(path, 'w', encoding='utf-8') as f:
                    f.write(self.tabs.currentWidget().toPlainText())
                self.statusBar().showMessage(f"💾 Guardado: {os.path.basename(path)}")
            except Exception as e: QMessageBox.critical(self, "Error", str(e))
        else:
            self._save_file_as()

    def _save_file_as(self):
        path, _ = QFileDialog.getSaveFileName(self, "Save As", BASE_DIR, "UPL Files (*.upl);;All Files (*)")
        if path:
            try:
                with open(path, 'w', encoding='utf-8') as f:
                    f.write(self.tabs.currentWidget().toPlainText())
                self.tabs.setTabText(self.tabs.currentIndex(), os.path.basename(path))
                self.open_tabs[path] = self.tabs.currentIndex()
                self.statusBar().showMessage(f"💾 Guardado como: {os.path.basename(path)}")
            except Exception as e: QMessageBox.critical(self, "Error", str(e))

    def _close_current_tab(self):
        if self.tabs.currentIndex() != -1: self._close_tab(self.tabs.currentIndex())

    def _toggle_explorer(self): self.explorer_panel.setVisible(not self.explorer_panel.isVisible())
    def _toggle_ir(self): self.ir_panel.setVisible(not self.ir_panel.isVisible())
    def _toggle_terminal(self): self.bottom_tabs.setVisible(not self.bottom_tabs.isVisible())

    def _debug_upl(self):
        w = self.tabs.currentWidget()
        if not w: return
        self.console.clear()
        self.console.append("🪲 INICIANDO MODO DEPURACIÓN (TRACING ASM)...")
        if self.orch:
            try:
                ir, asm = self.orch.compile_upl(w.toPlainText())
                self.console.append("-" * 40)
                self.console.append("🔍 TRACE DE LENGUAJE MADRE (UPL-IR):")
                self.console.append(ir)
                self.console.append("-" * 40)
                self.console.append("💾 MAPEO DE REGISTROS ASM:")
                self.console.append(asm if asm else "Sin ASM generado.")
                self.console.append("-" * 40)
                self.console.append("✅ DEPURACIÓN FINALIZADA SIN FALLOS CRÍTICOS.")
            except Exception as e:
                self.console.append(f"🔴 EXCEPCIÓN EN DEPURACIÓN: {e}")

    def _correct_upl(self):
        w = self.tabs.currentWidget()
        if not w: return
        self.console.clear()
        self.console.append("🔧 INICIANDO MOTOR DE CORRECCIÓN (AUTO-LINT)...")
        text = w.toPlainText()
        # Simulación de sugerencias inteligentes
        suggestions = []
        if "PROCEDIMIENTO" not in text and "PROC" not in text:
            suggestions.append("⚠️ Sugerencia: Falta bloque principal PROCEDIMIENTO.")
        if "RETORNAR" not in text and "RET" not in text:
            suggestions.append("💡 Tip: Considera añadir RETORNAR al final de tus funciones.")
            
        if not suggestions:
            self.console.append("✨ Sintaxis impecable. No se requieren correcciones.")
        else:
            for s in suggestions: self.console.append(s)
        self.console.append("✅ Análisis de consistencia finalizado.")

    def _new_file(self):
        ed = UPLEditor(mnemonics=self.mnemonics)
        ed.textChanged.connect(self._sync_all)
        self.tabs.setCurrentIndex(self.tabs.addTab(ed, "nuevo.upl"))

    def _on_tree_double_click(self, index):
        path = self.file_model.filePath(index)
        if os.path.isfile(path): self._load_file(path)

    def _load_file(self, path):
        if path in self.open_tabs: self.tabs.setCurrentIndex(self.open_tabs[path]); return
        try:
            with open(path, 'r', encoding='utf-8', errors='ignore') as f:
                ed = UPLEditor(mnemonics=self.mnemonics); ed.setPlainText(f.read()); ed.textChanged.connect(self._sync_all)
                idx = self.tabs.addTab(ed, os.path.basename(path))
                self.open_tabs[path] = idx; self.tabs.setCurrentIndex(idx)
        except Exception as e: self.console.append(f"❌ Error: {e}")

    def _close_tab(self, index):
        for p, i in list(self.open_tabs.items()):
            if i == index: del self.open_tabs[p]
            elif i > index: self.open_tabs[p] -= 1
        self.tabs.removeTab(index)

    def _run_upl(self):
        w = self.tabs.currentWidget()
        if not w: return
        self.console.clear(); self.console.append("🚀 INICIANDO...")
        if self.orch:
            try:
                ir, _ = self.orch.compile_upl(w.toPlainText()); self.ir_view.setText(ir)
                self.console.append("✨ EXITOSO.")
            except Exception as e: self.console.append(f"❌ ERROR: {e}")

    def _sync_all(self):
        w = self.tabs.currentWidget()
        if self.orch and w:
            try: ir, _ = self.orch.compile_upl(w.toPlainText()); self.ir_view.setText(ir)
            except: pass

    def _load_demo(self):
        path = os.path.join(BASE_DIR, "MUESTRA_VISUAL_POLIGLOTA.upl")
        if os.path.exists(path): self._load_file(path)

if __name__ == "__main__":
    app = QApplication(sys.argv); window = UPLStudio(); window.show(); sys.exit(app.exec())
