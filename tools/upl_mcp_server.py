#!/usr/bin/env python3
"""
Servidor MCP (Model Context Protocol) para Universal Polyglot Layer (UPL)
Permite a Antigravity compilar, ejecutar, empaquetar y consultar módulos UPL de forma nativa.
"""

import sys
import json
import subprocess
import os

WORKSPACE_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
UPLC_BIN = os.path.join(WORKSPACE_DIR, "bin", "uplc.exe" if os.name == "nt" else "uplc")

def run_uplc(args):
    cmd = [UPLC_BIN] + args
    res = subprocess.run(cmd, cwd=WORKSPACE_DIR, capture_output=True, text=True)
    return {
        "exit_code": res.returncode,
        "stdout": res.stdout,
        "stderr": res.stderr
    }

def handle_list_tools():
    return {
        "tools": [
            {
                "name": "upl_run",
                "description": "Compila en memoria y ejecuta un archivo .upl inmediatamente.",
                "inputSchema": {
                    "type": "object",
                    "properties": {
                        "file_path": {"type": "string", "description": "Ruta al archivo .upl"}
                    },
                    "required": ["file_path"]
                }
            },
            {
                "name": "upl_build",
                "description": "Compila un archivo .upl a ejecutable nativo, C99 o ensamblador NASM.",
                "inputSchema": {
                    "type": "object",
                    "properties": {
                        "file_path": {"type": "string", "description": "Ruta al archivo .upl"},
                        "target": {"type": "string", "description": "Target: bin, c, asm, deb, rpm, app, apk", "default": "bin"},
                        "output_file": {"type": "string", "description": "Ruta del binario o archivo de salida"}
                    },
                    "required": ["file_path"]
                }
            },
            {
                "name": "upl_list_modules",
                "description": "Lista todos los módulos de sistemas registrados en el ecosistema UPL.",
                "inputSchema": {
                    "type": "object",
                    "properties": {}
                }
            }
        ]
    }

def handle_call_tool(name, arguments):
    if name == "upl_run":
        fpath = arguments.get("file_path")
        res = run_uplc(["run", fpath])
        return {
            "content": [
                {
                    "type": "text",
                    "text": f"Exit Code: {res['exit_code']}\nStdout:\n{res['stdout']}\nStderr:\n{res['stderr']}"
                }
            ]
        }
    elif name == "upl_build":
        fpath = arguments.get("file_path")
        target = arguments.get("target", "bin")
        out = arguments.get("output_file")
        args = ["build", fpath, "-t", target]
        if out:
            args.extend(["-o", out])
        res = run_uplc(args)
        return {
            "content": [
                {
                    "type": "text",
                    "text": f"Exit Code: {res['exit_code']}\nStdout:\n{res['stdout']}\nStderr:\n{res['stderr']}"
                }
            ]
        }
    elif name == "upl_list_modules":
        res = run_uplc(["list"])
        return {
            "content": [
                {
                    "type": "text",
                    "text": res["stdout"]
                }
            ]
        }
    else:
        return {"isError": True, "content": [{"type": "text", "text": f"Tool '{name}' no encontrada."}]}

def main():
    while True:
        line = sys.stdin.readline()
        if not line:
            break
        try:
            req = json.loads(line)
            method = req.get("method")
            req_id = req.get("id")

            if method == "initialize":
                res = {
                    "jsonrpc": "2.0",
                    "id": req_id,
                    "result": {
                        "protocolVersion": "2024-11-05",
                        "capabilities": {"tools": {}},
                        "serverInfo": {"name": "upl-mcp-server", "version": "1.5.0"}
                    }
                }
            elif method == "tools/list":
                tools = handle_list_tools()
                res = {"jsonrpc": "2.0", "id": req_id, "result": tools}
            elif method == "tools/call":
                params = req.get("params", {})
                t_name = params.get("name")
                t_args = params.get("arguments", {})
                result = handle_call_tool(t_name, t_args)
                res = {"jsonrpc": "2.0", "id": req_id, "result": result}
            else:
                res = {"jsonrpc": "2.0", "id": req_id, "result": {}}

            sys.stdout.write(json.dumps(res) + "\n")
            sys.stdout.flush()
        except Exception as e:
            err_res = {
                "jsonrpc": "2.0",
                "id": req.get("id") if "req" in locals() else None,
                "error": {"code": -32603, "message": str(e)}
            }
            sys.stdout.write(json.dumps(err_res) + "\n")
            sys.stdout.flush()

if __name__ == "__main__":
    main()
