import h5py
import numpy as np
from tqdm import tqdm
import pysort.particle_sort as ps

class TrackParticles(object):    
    def __init__(self, wkdir):
        self.wkdir = wkdir
        particle_dir = "/MS/RAW/electrons"
        self.track = ps.TrackParticles(wkdir + particle_dir)
        self.getTimes()
        return

    def __getitem__(self, key):
        return self.track.get_data(key)

    def iter_all(self):
        filenames = self.filenames
        for filename in tqdm(filenames):
            self.track.iter(filename)

    def getTimes(self):
        filenames = self.filenames
        _times = []
        for filename in filenames:
            with h5py.File(filename, "r") as f:
                idx = f.attrs["ITER"]
                dt = f["SIMULATION"].attrs["DT"]
                _times.append(idx[0] * dt[0])
        self._times = np.asarray(_times)
        return

    @property
    def filenames(self):
        return self.track.get_filenames()
    
    @property
    def times(self):
        return self._times