# Readme generation
For the reason that github does not support equations, the tool readme2tex is used to embed LaTeX equation into markdown. The tool readme2tex converts the equations therefore to images preferably to SVG images.

# Installation 
```bash
pip install --user readme2tex
```
# Usage
For local usage for example with Visual Studio Code, in this case relative paths are used.
```bash
cd particle_swarm_optimization
python -m readme2tex docs/README_BASE.md --output README.md --nocdn --svgdir docs/images/readme2tex
```

For use on github

```bash
cd particle_swarm_optimization
python -m readme2tex docs/README_BASE.md --output README.md --svgdir docs/images/readme2tex
```

#Notes
In the case that the current directory is not an git repository it is possible to specify the options:
* --username kolb-stefan
* --project particle_swarm_optimization

The tool readme2tex will show an error in this case but the output is generated never the less.

