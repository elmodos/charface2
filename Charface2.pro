TEMPLATE = subdirs

SUBDIRS += \
    charface
    # TODO: Adopt plugins
    #\
    #PluginDemoImport \
    #PluginCuneiformOCR \
    #PluginScan \
    #PluginColorEdit \
    #PluginAnalyze

#!Q_WS_MACX {
#    SUBDIRS += PluginPDFImport \
#        PluginDjvuImport \
#        PluginMultipageTiffImport
#}
