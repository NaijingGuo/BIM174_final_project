from sqlalchemy import Column, String, Numeric, Integer

from database import Base


class Data(Base):
    __tablename__ = "data"

    id = Column(Integer, primary_key=True, index=True)
    temprature = Column(String)
    humidity = Column(String)
