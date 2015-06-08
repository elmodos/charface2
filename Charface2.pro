TEMPLATE = subdirs

SUBDIRS += \
    charface
    # TODO: Adopt plugins
    # plugins/PluginDemoImport \
    # plugins/PluginCuneiformOCR \
    # plugins/PluginScan \
    # plugins/PluginColorEdit \
    # plugins/PluginAnalyze

!macosx {
    # SUBDIRS += \
	# plugins/PluginPDFImport \
	# plugins/PluginDjvuImport \
	# plugins/PluginMultipageTiffImport
}
