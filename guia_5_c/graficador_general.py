import numpy as np
import pandas as pd
import os
import seaborn as sns


carpetas = [entrada.name for entrada in os.scandir() if entrada.is_dir()];

for carpeta in carpetas:

    print(f'Estamos en la carpeta {carpeta}');

    for 

