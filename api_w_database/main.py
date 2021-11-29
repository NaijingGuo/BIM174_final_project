from typing import Optional

from fastapi import FastAPI, Request, Depends
from pydantic import BaseModel
from fastapi.templating import Jinja2Templates
import models
from database import SessionLocal, engine
from sqlalchemy.orm import Session
from models import Data


class Item(BaseModel):
    name: str
    description: Optional[str] = None
    temp: str
    humidity: Optional[str] = None


models.Base.metadata.create_all(bind=engine)

app = FastAPI()
templates = Jinja2Templates(directory='templates')


def get_db():
    try:
        db = SessionLocal()
        yield db
    finally:
        db.close


@app.get('/')
async def home(request: Request, db: Session = Depends(get_db)):
    data = db.query(Data).all()
    data = data[-1]
    return templates.TemplateResponse("home.html", {"request": request, "data": data})


@ app.post("/items/")
async def create_item(item: Item, db: Session = Depends(get_db)):
    data = Data()
    data.temprature = item.temp
    data.humidity = item.humidity
    db.add(data)
    db.commit()
    # print(item)
    return item
