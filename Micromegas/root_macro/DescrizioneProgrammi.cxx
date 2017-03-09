planarity_plates_gauge (bool plates, bool stiffback,  string scan)
- plot per mappa di piastre o pcb
- sia tavolo che stiffback
- fornire .txt di scan 
- produce .root file

planarity_plates_laser (bool plates,bool stiffback, string scan)
- plot per mappa di piastre o pcb
- sia tavolo che stiffback
- fornire .txt di scan
- produce .root file

diff_pcb_piastre (bool laser, TString scan)
- fa la differenza fra run analizzato (fornire .root generato con planarity_plates_* )  e un reference ( o un altro .root (hardcoded)
- bool laser per la scelta del reference

planarity_diag (int p, string scan1, string scan2)
- p indica quale diagonale ( p=1 frame sx, p=2 frame dx, p=3 masks sx, p=4 masks dx);
- scan1 e scan2 sono i txt di due run;


