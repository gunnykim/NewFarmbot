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

if __name__ == "__main__":  # 웹사이트를 호스팅하여 접속자에게 보여주기 위한 부분
   app.run(host="192.168.0.2", port = "8080")
   # host는 현재 라즈베리파이의 내부 IP(192.168.0.2), port는 임의로 8080으로 설정
   # 해당 내부 IP와 port를 포트포워딩 해두면 외부에서도 접속가능하지만
   # 학교에서 외부 접근 막음, 현재 같은 공유기 내에서만 접속가능








 