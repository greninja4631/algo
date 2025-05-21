from fastapi import FastAPI
from pydantic import BaseModel

app = FastAPI()
state = {"message": "Hello, world!"}

class Message(BaseModel):
    new_message: str

@app.get("/message")
def get_message():
    return state

@app.post("/message")
def set_message(msg: Message):
    state["message"] = msg.new_message
    return state

