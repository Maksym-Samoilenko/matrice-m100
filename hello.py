from flask import Flask
from flask import render_template
import pexpect
app = Flask(__name__)
child = pexpect.spawn('./DJI_Onboard_API_Cmdline_Test')
isTakeoff = False
@app.route('/')
def index():
    return render_template('index.html')

@app.route('/takeoff')
def takeoff():
    if not isTakeoff:
        child.sendline ('a')
        child.expect ('input')
        child.sendline ('b')
        child.expect ('input')
        child.sendline ('d')
        isTakeoff = True
        return 'Takeoff OK'
    else:
        return 'Takeoff refused'

@app.route('/landing')
def landing():
    if isTakeoff:
        child.sendline ('e')
        isTakeoff = False
        return 'Landing OK'
    else:
        return 'Landing refused'

if __name__ == '__main__':
    app.run()
