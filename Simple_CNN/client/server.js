const express = require('express');
const server = express();
const path = require('path');
const fabric = require('fabric').fabric;
const fs = require('fs');
const { createCanvas, loadImage } = require('canvas');
const sharp = require('sharp');
const cp = require('child_process');

server.listen(50088);

server.use(express.static(path.join(__dirname, 'html')));
server.use(express.static(path.join(__dirname, 'js')));

server.use(express.json())
server.post('/json', function(req, res) {
    var canvas = new fabric.Canvas(null, {width: 560, height: 560})
    //console.log(req.body);
    canvas.loadFromJSON(req.body, function() {
        canvas.renderAll();
        //console.log(canvas.getContext().getImageData(0,0,560,560));
        /*
        var out = fs.createWriteStream(__dirname + '/hello.png');
        var stream = canvas.createPNGStream();
        stream.on('data', function(chunk) {
            out.write(chunk);
        });
        */
        /*
        var filter = new fabric.Image.filters.Resize();
        var img = filter.bilinearFiltering({
            imageData: canvas.getContext().getImageData(0,0,560,560),
            ctx: canvas.getContext()
        }, 560, 560, 28, 28);
        console.log(img);

        var c1 = new fabric.Canvas(null, {width: 28, height: 28});
        c1.getContext().putImageData(img,0,0);
        var out = fs.createWriteStream(__dirname + '/hello.png');
        var stream = c1.createPNGStream();
        stream.on('data', function(chunk) {
            out.write(chunk);
        });
        */
        var pngStr = canvas.toDataURL();
        var regex = /^data:.+\/(.+);base64,(.*)$/;
        var matches = pngStr.match(regex);
        var ext = matches[1];
        var data = matches[2];
        var buffer = Buffer.from(data, 'base64');
        var ret;
        sharp(buffer).resize(28,28).grayscale().raw({ resolveWithObject: true })
        .toBuffer().then(function(pixels) {
            //console.log(pixels.toString('hex'));
            var inputStr = pixels.join(' ');
            console.log(inputStr);
	
            var child = cp.spawn('./bin/scnn', ['./bin/digit_784_20_10.nn']);
            //var child = cp.spawn('./bin/test');
            //child.stdin.write(inputStr);
            child.on('exit', function(code) {
                console.log('exit');
                res.send({msg: ret});
            })
            child.stdout.on('data', function(data) {
                console.log(data);
                ret = data.toString();
                //res.send(data);
            })
            child.stdin.on('error', function(error) {console.log(error);}); 
            child.stdin.write(inputStr);
            child.stdin.end();
	
        })
        //res.send({msg: "10"});
    })
})

console.log('Server listening port 50088');
