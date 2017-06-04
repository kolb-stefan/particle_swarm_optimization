# Readme generation
For the reason that github does not support equations, the tool readme2tex is used to embed LaTeX equation into markdown. The tool readme2tex converts the equations therefore to images preferably to SVG images.

# Installation 
```bash
pip install --user readme2tex
```
If the version provided by pip is not new enough it is possible to install readme2tex directly form github.
```bash
pip install --user git+https://github.com/leegao/readme2tex.git
```

# Usage
For local usage for example with Visual Studio Code, relative paths are used. This setting should also work with github.
```bash
cd particle_swarm_optimization
python -m readme2tex docs/README_BASE.md --output README.md --nocdn --pngtrick --svgdir docs/images/readme2tex
```

# Notes
In the case that the current directory is not an git repository it is possible to specify the options:
* --username kolb-stefan
* --project particle_swarm_optimization

The tool readme2tex will show an error in this case but the output is generated never the less.

