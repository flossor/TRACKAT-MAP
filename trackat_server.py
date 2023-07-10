from flask import Flask, request
from functools import wraps
import jwt
from twilio import twiml
import json


app = Flask(__name__)

app.config['SECRET_KEY'] = 'kat'


# def token_required(f):
#     @wraps(f)
#     def decorated(*args,**kwargs):
#         token = request.args.get('token')
        
#45.547642, -73.578529
#45.56610413809432, -73.55540981666361
@app.route('/receive_data',methods=['GET','POST'])
def sms():
    # number = request.form['FROM']
    # body = request.form["BODY"]
    #print(request.form['Body'])
    if request.form:
        body = request.form["Body"]
        print(body)

    info = {"id":"38547r8e","lat":45.609119,"long":-73.527718,"battery":80}
   


    return info

@app.route('/locations', methods=['GET', 'POST'])
def locations():
    
    if request.form:
        pass

def save_location():
    pass

if __name__ == "__main__":
    app.run(debug=True)