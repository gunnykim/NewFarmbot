#
#   라즈베리파이 web server에 항상 띄워지는 web site
#
from flask import Flask, render_template, url_for, redirect   # flask 모듈과 관련함수 import
import os
app = Flask(__name__, template_folder="templates")            # Flask라는 이름의 객체 생성


# static files versioning 
def dated_url_for(endpoint, **values):
    if endpoint == 'static':
        filename = values.get('filename', None)
        if filename:
            file_path = os.path.join(app.root_path,
                                     endpoint, filename)
            values['q'] = int(os.stat(file_path).st_mtime)
    return url_for(endpoint, **values)
@app.context_processor
def override_url_for():
    return dict(url_for=dated_url_for)


@app.route('/')                     # 기본 주소
def home():                         # 여기서 index.html을 화면에 보여줌
    return render_template('index.html')  

@app.route('/<action>')  #팜봇의 기능 주소
def func(action):        #팜봇의 기능 <action> = 'water', 'home', 'cam'
    #actual farmbot control     ex) "192.168.0.88/water", "192.168.0.88/home", "192.168.0.?/cam"
    if action=='cam':
        url = "curl \"http://192.168.0.?/" + action + "\""
    else:
        url = "curl \"http://192.168.0.88/" + action + "\""
    os.system(url)
    return redirect(url_for('home'))    # 실행이 끝나면 기본주소로 돌아감

@app.route('/<action>/<int:num>')  #팜봇의 움직임 주소
def move(action, num):             #팜봇의 움직임 <action> = 'go' / <num> = 1, 2, 3, 4
    #actual farmbot control     ex) "192.168.0.88/go?num=1"
    url = "curl \"http://192.168.0.88/" + action + "?num=" + str(num) + "\""
    os.system(url)

    #virtual farmbot control 
    return redirect(url_for('home'))    # 실행이 끝나면 기본주소로 돌아감

if __name__ == "__main__":  # 웹사이트를 호스팅하여 접속자에게 보여주기 위한 부분
   app.run(host="192.168.0.2", port = "8080")
   # host는 현재 라즈베리파이의 내부 IP(192.168.0.2), port는 임의로 8080으로 설정
   # 해당 내부 IP와 port를 포트포워딩 해두면 외부에서도 접속가능하지만
   # 학교에서 외부 접근 막음, 현재 같은 공유기 내에서만 접속가능








 