# ChronoVolume — plugin OpenFX « cube de temps 3D »

Implémentation originale (C++17, CPU multithread). Elle empile les images d'un clip en un volume 3D
et le rend avec une caméra virtuelle. Nom volontairement différent du plugin commercial dont elle
s'inspire : aucun code ni asset n'en est repris.

## Fonctions incluses
- **4 modes** : Solid, Translucent Volume, Separated Slices, Cross Section (plan oblique = slit-scan)
- **Motion Only** (fond = médiane temporelle), opacité depuis luma / max / saturation / chroma key / alpha
- **Courbure** : Bend 0→1 (arche → anneau temporel fermé)
- **Voxel Dust** (nuage de points), **Scan Lines** (lidar), teinte par âge
- **Trail Fade**, Frame Before/After, Loop, Flip, Frame Blend
- **Finitions** : glow, aberration chromatique, grain, couleur de fond
- **9 presets** (appliquent des valeurs, tout reste modifiable/animable)
- Caméra orbitale : yaw, pitch, FOV, zoom

## Non inclus (honnêtement)
- Flux optique (Frame Blend est un simple fondu entre images voisines)
- Shaders verre / verre dépoli (réfraction, iridescence), sol studio avec ombres et illumination globale, occlusion ambiante
- GPU (CUDA/Metal/OpenCL) : le rendu CPU est lent en 4K avec beaucoup d'images ; baisser « Quality »
- Les 17 presets d'origine (ici 9, avec mes propres valeurs)

## État de vérification
- `chrono_core.h` (tout le rendu) est compilé et testé en autonome : `g++ -O2 -std=c++17 test_core.cpp -pthread && ./a.out`
- `ChronoVolume.cpp` (colle OpenFX) **n'a pas pu être compilé ici** (pas de SDK, pas de réseau). Écrit pour la
  Support library OpenFX 1.4 ; si ta version a renommé un appel (ex. `FramesNeededSetter`), il faudra l'ajuster.

## Compilation
```
git clone https://github.com/AcademySoftwareFoundation/openfx
cmake -B build -DOFX_SDK=/chemin/openfx -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```
Le bundle `build/ChronoVolume.ofx.bundle` est à copier dans :
- Windows : `C:\Program Files\Common Files\OFX\Plugins\`
- macOS : `/Library/OFX/Plugins/` (signer le bundle : `codesign --force --deep -s - ChronoVolume.ofx.bundle`)
- Linux : `/usr/OFX/Plugins/`

Hôtes visés : DaVinci Resolve (Studio), Natron, Nuke, Fusion, Vegas.

## Notes d'utilisation
- Image avant = la plus récente ; « Frames Before » = nombre d'images passées dans le bloc.
- Presets : *Ghost Trail* et *Motion Sculpture* donnent le plus vite un résultat lisible sur un sujet qui bouge devant un fond fixe.
- Pas de support des tuiles : l'hôte rend l'image entière.
