from sqlalchemy import create_engine, ForeignKey, Column, String, Integer, CHAR, FLOAT, DateTime
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import sessionmaker


base = declarative_base()


class Profile(base):
    __tablename__ = "profiles"

    profile_id = Column("profile_id", Integer, primary_key=True)
    first_name = Column("first_name", String)
    last_name = Column("last_name", String)
    hash = Column("hash", String)

    def __init__(self, profile_id, first_name, last_name,hash):
        self.profile_id = profile_id
        self.first_name = first_name
        self.last_name = last_name
        self.hash = hash


class Device(base):
    __tablename__ = "devices"

    device_id = Column("device_id", Integer, primary_key=True)
    sim_number = Column("number", Integer)
    name = Column("name", String)
    profile_id = Column(Integer, ForeignKey("profiles.profile_id"))

    def __init__(self, device_id, sim_number, name, profile_id):
        self.device_id = device_id
        self.sim_number = sim_number
        self.name = name
        self.profile_id = profile_id
    
    def __repr__(self):
        return f"({self.device_id}) {self.sim_number} {self.name}"

class Location(base):
    __tablename__ = "locations"

    location_id = Column("location_id",Integer,primary_key=True)
    lattitude = Column("lattitude", FLOAT)
    longitude = Column("longitude", FLOAT)
    device_id = Column(Integer, ForeignKey("devices.device_id"))
    time = Column("time", DateTime)

    def __init__(self, location_id, lattitude, longitude, device_id, time):
        self.location_id = location_id
        self.lattitude = lattitude
        self.longitude = longitude
        self.device_id = device_id
        self.time = time


    def __repr__(self):
        return f"({self.location_id}) {self.lattitude}, {self.longitude} [{self.device_id} @ {self.time}]"

engine = create_engine("sqlite:///trackat.db", echo=True)
base.metadata.create_all(bind=engine)
Session = sessionmaker(bind=engine)
session = Session()


def get_devices(profile_id): 
    """
    Get all devices linked to an account
    """
    devices = session.query(Device).filter(Device.profile_id == profile_id)
    return devices

def add_device(profile_id, device_id):
    pass
def get_locations(device_id:int):
    locations = session.query(Location).filter(Location.device_id == device_id)
    return locations
def add_location(device_id, latitude, longitude, time):
    location = Location(0,latitude,longitude,device_id,time)
    session.add(location)
    session.commit()