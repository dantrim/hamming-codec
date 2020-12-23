#!/bin/bash
unset PYTHONPATH
export PYTHONPATH=$(python -c 'import sysconfig; print(sysconfig.get_paths()["purelib"])')
echo "PYTHONPATH set to \"${PYTHONPATH}\""
