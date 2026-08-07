#
# conf.py
#
# Copyright The OBDH 2.0 Contributors.
#
# OBDH 2.0 Documentation
#
# This work is licensed under the Creative Commons Attribution-ShareAlike 4.0
# International License. To view a copy of this license,
# visit http://creativecommons.org/licenses/by-sa/4.0/.
#

import ast
import sys

from docutils import nodes
from sphinx.builders.latex.nodes import thebibliography
from sphinx.transforms.post_transforms import SphinxPostTransform


class PlaceBibliographyAtMarker(SphinxPostTransform):
    """Keep the PDF bibliography before the appendices.

    Sphinx's LaTeX builder normally gathers every citation at the very end of
    the document. The manual intentionally places References before its
    appendices, as the original LaTeX source did.
    """

    default_priority = 760
    formats = ("latex",)

    def run(self, **kwargs):
        bibliographies = list(self.document.findall(thebibliography))
        if not bibliographies:
            return

        for marker in self.document.findall(nodes.raw):
            if "OBDH_BIBLIOGRAPHY_MARKER" in marker.astext():
                bibliography = bibliographies[0]
                bibliography.parent.remove(bibliography)
                marker.replace_self(bibliography)
                return


class UseCompactPacketSummary(SphinxPostTransform):
    """Use the original compact landscape layout for the PDF packet table."""

    default_priority = 760
    formats = ("latex",)

    def run(self, **kwargs):
        for table in self.document.findall(nodes.table):
            if "packet-summary" in table.get("classes", []):
                table.replace_self(
                    nodes.raw(
                        "",
                        r"\input{packet_summary_table.inc}",
                        format="latex",
                    )
                )

# Project information
project = "obdh2"
copyright = "The OBDH 2.0 Contributors"
author = "SpaceLab"
release = "v1.0"
title = "OBDH 2.0 Documentation"
doc_id = "slb-obdh2-doc"

# General configuration
numfig = True
latex_table_style = ["booktabs", "colorrows"]

extensions = ["sphinxcontrib.bibtex", "sphinx_subfigure"]

bibtex_bibfiles = ["references.bib"]

templates_path = ["_templates"]
exclude_patterns = ["_build", ".venv", "Thumbs.db", ".DS_Store"]

# Identify the Sphinx builder being used.
if "-b" in sys.argv:
    builder = sys.argv[sys.argv.index("-b") + 1]
elif "-M" in sys.argv:
    builder = sys.argv[sys.argv.index("-M") + 1]
else:
    builder = "html"

# Exclude the PDF-specific index from HTML builds.
if builder in ["html", "dirhtml"]:
    exclude_patterns.append("pdf-index.rst")
elif builder in ["latex", "latexpdf"]:
    exclude_patterns.append("index.rst")
    root_doc = "pdf-index"

# Options for HTML output
html_theme = "sphinx_rtd_theme"
html_static_path = ["assets"]
html_css_files = ["css/custom.css"]
html_title = "OBDH 2.0 Documentation"
html_short_title = "OBDH 2.0 Documentation"

# PDF output configuration
latex_documents = [
    (
        "pdf-index",
        doc_id + "-" + release + ".tex",
        title,
        author,
        "manual",
        False,
    ),
]

latex_toplevel_sectioning = "chapter"
latex_show_pagerefs = True
latex_show_urls = "footnote"

latex_elements_file = "latex/latex_elements_custom.txt"

latex_elements = {}
with open(latex_elements_file, "rt", encoding="utf-8") as file:
    latex_elements = ast.literal_eval(file.read())

latex_additional_files = [
    "latex/spacelab_book.sty",
    "latex/packet_summary_table.inc",
    "img/by-sa.pdf",
    "img/spacelab-logo-full-color-rgb-1000px@72ppi.png",
]


def setup(app):
    app.add_post_transform(PlaceBibliographyAtMarker)
    app.add_post_transform(UseCompactPacketSummary)
