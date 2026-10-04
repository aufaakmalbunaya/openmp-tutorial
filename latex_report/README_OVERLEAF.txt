LAPORAN PRAKTIKUM TUTORIAL OPENMP — PAKET OVERLEAF
================================================
Nama: Aufa Akmal Bunaya
Tanggal: 4 Oktober 2026

ISI PAKET
---------
laporan_openmp.tex   : sumber LaTeX laporan (bahasa Indonesia)
screenshots/         : 10 screenshot bukti eksekusi (shot_01 - shot_10)
pi_runtime.png       : plot runtime vs jumlah thread (Exercise 8)
pi_speedup.png       : plot speedup vs jumlah thread (Exercise 8)

CARA KOMPILASI DI OVERLEAF
--------------------------
1. Upload laporan_openmp.tex sebagai main file.
2. Upload seluruh isi screenshots/ dan kedua file pi_*.png
   ke folder yang sama dengan file .tex (atau sesuaikan path).
3. Pilih compiler pdfLaTeX, lalu Recompile (2x untuk TOC final).

CATATAN
-------
- File .tex memakai paket standar: babel (indonesian, provide=*),
  lmodern, microtype, xcolor, graphicx, booktabs, tabularx,
  listings, soul, hyperref, geometry.
- Seluruh gambar dirujuk dengan path relatif tanpa subfolder
  (shot_01_env.png, ..., pi_runtime.png, pi_speedup.png).
- Kode sumber C lengkap ada di repositori:
  https://github.com/aufaakmalbunaya/openmp-tutorial
