import json
from flask import Flask, render_template
app=Flask(__name__)

@app.route('/')
def hello_world():
    
    return render_template('index.html')

app.run(debug=True,host='localhost',port=5000)
#app.run(host='0.0.0.0') 
