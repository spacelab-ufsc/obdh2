<h1 align="center">
    OBDH2 DOCUMENTATION
    <br>
</h1>

<h4 align="center">Documentation of the OBDH2 Module.</h4>

<p align="center">
    <a href="https://github.com/spacelab-ufsc/spacelab#versioning">
        <img src="https://img.shields.io/badge/status-in%20development-red?style=for-the-badge">
    </a>
    <a href="https://github.com/spacelab-ufsc/obdh2/releases">
        <img alt="GitHub release (latest by date)" src="https://img.shields.io/github/v/release/spacelab-ufsc/obdh2?style=for-the-badge">
    </a>
    <a href="https://github.com/spacelab-ufsc/obdh2/actions">
        <img alt="Documentation build" src="https://img.shields.io/github/actions/workflow/status/spacelab-ufsc/obdh2/doc_build.yml?style=for-the-badge">
    </a>
    <a href="">
        <img src="https://img.shields.io/badge/DOC%20tool-Sphinx-yellow?style=for-the-badge">
    </a>
    <a href="#license">
        <img src="https://img.shields.io/badge/LICENSE-CC%20BY--SA%204.0-yellow?style=for-the-badge">
    </a>
</p>

<p align="center">
    <a href="#dependencies">Dependencies</a> •
    <a href="#building">Building</a> •
    <a href="#license">License</a>
</p>

## Dependencies

The Python dependencies required to build this documentation are listed in [`requirements.txt`](requirements.txt):

* [sphinx-rtd-theme](https://pypi.org/project/sphinx-rtd-theme/)
* [sphinxcontrib-bibtex](https://pypi.org/project/sphinxcontrib-bibtex/)
* [sphinx-subfigure](https://pypi.org/project/sphinx-subfigure/)

## Building

After installing the dependencies, generate the HTML documentation with:

```sh
make html
```

### Generating the PDF file

With a LaTeX toolchain installed, run:

```sh
make latexpdf
```

## License

This documentation is licensed under the Creative Commons Attribution-ShareAlike 4.0 International license (CC BY-SA 4.0).
