import requests
import time
while True:
    print "This prints once a minute."
    r = requests.get('http://10.17.11.129:5000/telemetry_data_gps')
    print(r.text)
    payload = {'command': 'bdupdate'}
    r = requests.get('https://script.google.com/macros/s/AKfycbxZN7CczmOQAUhVB8QcT0rJ8XWtAzC5xF7oe_E5sfOXEsU-uXhP/exec', params=payload)
    print r.json()
    payloadcmdget = {'command': 'cmdget'}
    c = requests.get('https://script.google.com/macros/s/AKfycbxZN7CczmOQAUhVB8QcT0rJ8XWtAzC5xF7oe_E5sfOXEsU-uXhP/exec', params=payloadcmdget)
    command = c.json()['cmd']
    if (command =="activation"):
        r = requests.get('10.17.11.129:5000/activate')
    elif (command == "obtain"):
        r = requests.get('10.17.11.129:5000/obtain_control')
    elif (command == "release"):
        r = requests.get('10.17.11.129:5000/release_control')
    elif (command == "takeoff"):
        r = requests.get('10.17.11.129:5000/takeoff')
    elif (command == "landing"):
        r = requests.get('10.17.11.129:5000/landing')
    #r get status
    #do smth
    #r push status
    time.sleep(10)  # Delay for 1 minute (60 seconds)
