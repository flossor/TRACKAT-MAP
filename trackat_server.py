from flask import Flask, request
from functools import wraps
import jwt
from twilio import twiml
import json
import datetime

import TK_database as database


app = Flask(__name__)

app.config['SECRET_KEY'] = 'kat'


# def token_required(f):
#     @wraps(f)
#     def decorated(*args,**kwargs):
#         token = request.args.get('token')

@app.route("/", methods=['GET'])
def home():
    return "<h1>TRACKAT API</h1>"
        
@app.route('/receive_data',methods=['GET','POST'])
def sms():
    # number = request.form['FROM']

    if request.form:
        body = request.form["Body"]
        print(body)
        info = json.loads(body)
        save_location(info)

        return 200
   

    return 400

@app.route('/locations', methods=['GET', 'POST'])
def locations():
    '''
    Get the list of all locations
    '''

    locations = database.get_locations(0)
    locations_response = {}

    for i in range(locations.count()):
        locations_response[str(i+1)] = {"date" : locations[i].time, "latitude" : locations[i].lattitude, "longitude" : locations[i].longitude}


    return locations_response

def save_location(info):
    '''
    Save location to the database
    '''

    database.add_location(info["id"],info["lat"],info["long"], datetime.datetime.now())
    

if __name__ == "__main__":
    app.run(debug=True)