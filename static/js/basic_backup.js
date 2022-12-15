import * as THREE from 'three';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls';
import { GLTFLoader } from 'three/examples/jsm/loaders/GLTFLoader';
import Stats from 'three/examples/jsm/libs/stats';


class App {
    constructor() {
        const divContainer = document.querySelector("#webgl-container");
        this._divContainer = divContainer;

        const renderer = new THREE.WebGLRenderer({ antialias: true });
        renderer.setPixelRatio(window.devicePixelRatio);
        divContainer.appendChild(renderer.domElement);

        renderer.shadowMap.enabled = true;
        renderer.shadowMap.type = THREE.VSMShadowMap;

        this._renderer = renderer;

        const scene = new THREE.Scene();
        this._scene = scene;

        this._setupCamera();
        this._setupLight();
        this._setupModel();
        this._setupControls();

        window.onresize = this.resize.bind(this);
        this.resize();

        requestAnimationFrame(this.render.bind(this));
    }

    _setupControls() {
        this._controls = new OrbitControls(this._camera, this._divContainer);
        this._controls.target.set(0, 100, 0);
        
        const stats = new Stats();
        this._divContainer.appendChild(stats.dom);
        this._fps = stats; 
        
        this._clickedBtns = {};

        document.getElementById("1").addEventListener("click", (event) => {
            this._clickedBtns["1"] = true;
            this._processAnimation();
        });

        document.getElementById("2").addEventListener("click", (event) => {
            this._clickedBtns["2"] = true;
            this._processAnimation();
        });

        document.getElementById("3").addEventListener("click", (event) => {
            this._clickedBtns["3"] = true;
            this._processAnimation();
        });

        document.getElementById("4").addEventListener("click", (event) => {
            this._clickedBtns["4"] = true;
            this._processAnimation();
        });

        document.getElementById("H").addEventListener("click", (event) => {
            this._clickedBtns["H"] = true;
            this._processAnimation();
        });

        document.getElementById("water").addEventListener("click", (event) => {
            this._clickedBtns["water"] = true;
            this._processAnimation();
        });

        document.getElementById("cam").addEventListener("click", (event) => {
            this._clickedBtns["cam"] = true;
            this._processAnimation();
        });
    }
    
    _processAnimation() {
        const previousAnimationAction = this._currentAnimationAction;

        if(this._move_flag==0){
            if(this._clickedBtns["1"]){
                this._g1_flag = 1;
                this._targetX = -1000;
                this._targetZ = -400;
                this._move_flag = 1;
                this._clickedBtns["1"] = false;
            }
            else if(this._clickedBtns["2"]){
                this._g2_flag = 1;
                this._targetX = -300;
                this._targetZ = -400;
                this._move_flag = 1;
                this._clickedBtns["2"] = false;
            }
            else if(this._clickedBtns["3"]){
                this._g3_flag = 1;
                this._targetX = -1000;
                this._targetZ = -1000;
                this._move_flag = 1;
                this._clickedBtns["3"] = false;
            }
            else if(this._clickedBtns["4"]){
                this._g4_flag = 1;
                this._targetX = -300;
                this._targetZ = -1000;
                this._move_flag = 1;
                this._clickedBtns["4"] = false;
            }                                                    
            else if(this._clickedBtns["H"]){
                this._h_flag = 1;
                this._targetX = 0;
                this._targetZ = 0;
                this._move_flag = 1;
                this._clickedBtns["H"] = false;
            }
            else if(this._clickedBtns["water"]){
                if(this._targetX!=0 && this._targetZ!=0){
                    this._w_flag = 1;
                    this._move_flag = 1;
                    this._clickedBtns["water"] = false;
                }    
            }
            else if(this._clickedBtns["cam"]){
                if(this._targetX!=0 && this._targetZ!=0){
                    this._c_flag = 1;
                    this._move_flag = 1;
                    this._clickedBtns["cam"] = false;
                }
            }

            if(previousAnimationAction !== this._currentAnimationAction) {
                previousAnimationAction.fadeOut(0.5);
                this._currentAnimationAction.reset().fadeIn(0.5).play();
            }
        }
    }

    _setupModel() {
        const planeGeometry = new THREE.PlaneGeometry(10000, 10000);
        const planeMaterial = new THREE.MeshPhongMaterial({ color: 0x878787 });
        const plane = new THREE.Mesh(planeGeometry, planeMaterial);
        plane.rotation.x = -Math.PI/2;
        this._scene.add(plane);
        plane.receiveShadow = true;

        const loader = new GLTFLoader();

        loader.load('static/data/gantry.glb', (gltf) => {
            const model_gan = gltf.scene;
            this._scene.add(model_gan);
            this._model_gan = model_gan;
        });

        loader.load('static/data/x_box.glb', (gltf) => {
            const model_x = gltf.scene;
            this._scene.add(model_x);
            this._model_x = model_x;
        });

        loader.load('static/data/z_plate.glb', (gltf) => {
            const model_z = gltf.scene;
            this._scene.add(model_z);
            this._model_z = model_z;
        });

        loader.load('static/data/y_axis.glb', (gltf) => {
            const model_y = gltf.scene;
            this._scene.add(model_y);
            this._model_y = model_y;
        });
    }

    _setupCamera() {
        const camera = new THREE.PerspectiveCamera(
            60, 
            window.innerWidth / window.innerHeight, 
            1, 
            50000
        );
        camera.position.set(0, 4000, 5000);
        this._camera = camera;
    }

    _addPointLight(x, y, z) {
        const color = 0xffffff;
        const intensity = 1.5;
        const pointLight = new THREE.PointLight(color, intensity, 2000);
        pointLight.position.set(x, y, z);
        this._scene.add(pointLight);
    }

    _setupLight() {
        const ambientLight = new THREE.AmbientLight(0xffffff, .5);
        this._scene.add(ambientLight);

        this._addPointLight(500, 150, 500);
        this._addPointLight(-500, 150, 500);
        this._addPointLight(-500, 150, -500);
        this._addPointLight(500, 150, -500);

        const shadowLight = new THREE.DirectionalLight(0xffffff, 0.2);
        shadowLight.position.set(200, 500, 200);
        shadowLight.target.position.set(0, 0, 0);
        
        this._scene.add(shadowLight);
        this._scene.add(shadowLight.target);

        shadowLight.castShadow = true;
        shadowLight.shadow.mapSize.width = 1024;
        shadowLight.shadow.mapSize.height = 1024;
        shadowLight.shadow.camera.top = shadowLight.shadow.camera.right = 700;
        shadowLight.shadow.camera.bottom = shadowLight.shadow.camera.left = -700;
        shadowLight.shadow.camera.near = 100;
        shadowLight.shadow.camera.far = 900;
        shadowLight.shadow.radius = 5;
    }    

    _speed = 0;
    _g1_flag = 0;
    _g2_flag = 0;
    _g3_flag = 0;
    _g4_flag = 0;
    _targetX = 0;
    _targetZ = 0;
    _h_flag = 0;
    _w_flag = 0;
    _c_flag = 0;
    _move_flag = 0;

    update(time) {
        time *= 0.001; // second unit
        this._controls.update();
        this._fps.update();
        //go
        if(this._g1_flag || this._g2_flag || this._g3_flag || this._g4_flag){
            //Y axis
            if( this._model_y.position.y<1000 &&
                (this._model_x.position.x!=this._targetX ||
                this._model_z.position.z!=this._targetZ)) 
            {
                this._model_y.position.y += 3;
                this._model_x.position.y += 3;
            }
            //X box
            else if(this._model_x.position.x!=this._targetX){
                if(this._model_x.position.x<this._targetX) this._model_x.position.x += 5;
                else this._model_x.position.x -= 5;
            }
            //Z plate
            else if(this._model_z.position.z!=this._targetZ){
                if(this._model_z.position.z<this._targetZ) this._model_z.position.z += 10;
                else this._model_z.position.z -= 10;
            }
            //Y axis
            else if(this._model_y.position.y>200){
                this._model_y.position.y -= 3;
                this._model_x.position.y -= 3;
            }
            else{
                this._g1_flag = 0;
                this._g2_flag = 0;
                this._g3_flag = 0;
                this._g4_flag = 0;
                this._targetX = 0;
                this._targetZ = 0;
                this._move_flag = 0;
            }
        }
        //Home
        if(this._h_flag){
            //Y axis
            if(this._model_y.position.y<1000 &&
                (this._model_x.position.x!=0 ||
                this._model_z.position.z!=0))
            {
                this._model_y.position.y += 3;
                this._model_x.position.y += 3;
            }
            //Z plate
            else if(this._model_z.position.z!=0){
                this._model_z.position.z += 10;
            }
            //X box
            else if(this._model_x.position.x!=0){
                this._model_x.position.x += 5;
            }
            //Y axis
            else if(this._model_y.position.y!=0){
                this._model_y.position.y -= 3;
                this._model_x.position.y -= 3;
            }
            else{
                this._h_flag = 0;
                this._targetX = 0;
                this._targetZ = 0;
                this._move_flag = 0;
            }
        }
        this._previousTime = time;
    }

    render(time) {
        this._renderer.render(this._scene, this._camera);   
        this.update(time);
        requestAnimationFrame(this.render.bind(this));
    }

    resize() {
        const width = this._divContainer.clientWidth;
        const height = this._divContainer.clientHeight;
        this._camera.aspect = width / height;
        this._camera.updateProjectionMatrix();
        this._renderer.setSize(width, height);
    }
}

window.onload = function () {
    new App();
}