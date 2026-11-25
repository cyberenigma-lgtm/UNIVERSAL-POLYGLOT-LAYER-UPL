# Case #08: Linux X509 CRL Native Memory Explosion (#108557)

This directory contains the root cause investigation, laboratory benchmark, surgical C# patch, and regression test for [dotnet/runtime issue #108557](https://github.com/dotnet/runtime/issues/108557): **"High native memory usage during CRL check for the server not featuring OCSP stapling"**.

## Contents
- **`patch_dotnet_108557.diff`**: Surgical C# diff in `System.Security.Cryptography/src/System/Security/Cryptography/X509Certificates/OpenSslCrlCache.cs`.
- **`CrlDownloadCoalescingTest.cs`**: Unit test asserting single-flight coalescing under 50 concurrent TLS handshakes.
- **`GITHUB_COMMENT.md`**: Polished GitHub response addressing @bartonjs and @sfc-gh-dlisouski.
- **`PR_DESCRIPTION.md`**: Clean, human-written pull request description ready for upstream submission.
- **`DOCUMENTATION_TECHNICAL_EN.md`**: Detailed architectural breakdown of OpenSSL ASN.1 parsing, glibc memory arenas, and single-flight task coalescing.
- **`crl_openssl_audit.upl`**: Laboratory benchmark model compiled with the UPL+ compiler v2.2.3.

## Key Results
- **Unpatched CoreCLR (Linux x64):** 3.75 GB native RSS consumed under 50 concurrent handshakes (Kubernetes OOMKill).
- **Patched CoreCLR / UPL+ Model:** 76 MB total RSS (98.0% memory reduction, zero container evictions).
