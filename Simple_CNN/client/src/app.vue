<template>
<v-app>
    <v-toolbar app>
        <v-toolbar-title class="headline text-uppercase">
            <span>Simple CNN</span>
        </v-toolbar-title>
        <v-spacer></v-spacer>
        <v-btn flat href="https://github.com/s5901370/Simple_CNN"
        target="_blank">
            <span class="mr-2">Latest Release</span>
        </v-btn>
    </v-toolbar>
    <v-content>
        <v-hover>
            <v-card
                slot-scope="{ hover }"
                :class="`elevation-${hover ? 12 : 2}`"
                class="mx-auto"
                width="560"
            >
                <div>
                    <canvas id="paint"></canvas>
                </div>
                <v-divider></v-divider>
                <v-card-title>
                    <h3 class="headline mb-0">Draw a number</h3>
                </v-card-title>
                <v-card-actions>
                    <v-dialog v-model="dialog" width="500">
                        <v-card>
                            <v-card-title>Number is {{number}}</v-card-title>
                        </v-card>
                    </v-dialog>
                    <v-btn dark @click="sendjson">Send </v-btn>
                    <v-btn flat color="orange" @click="reset">Reset</v-btn>
                </v-card-actions>
            </v-card>
        </v-hover>
    </v-content>
</v-app>
</template>

<script>
    export default {
        name: "App",
        data() {
            return {
                //text: "Hello, Vue!"
                canvas : undefined,
                number : "0",
                dialog : false
            };
        },
        methods: {
            test : function(event) {
                console.log(this.canvas.getContext()
                .getImageData(0,0,this.canvas.getWidth(),this.canvas.getHeight()))
            },
            sendjson : function(event) {
                console.log(JSON.stringify(this.canvas))
                console.log(this.canvas.toJSON())
                var a = this;
                $.ajax({
                    type: 'POST',
                    url: '/json',
                    data: JSON.stringify(this.canvas),
                    success: function(data) { console.log('data: ' + data.msg); a.number = data.msg, a.dialog = true},
                    contentType: "application/json",
                    dataType: 'json'
                });
                
            },
            reset : function(event) {
                this.canvas.clear();
            }
        },
        mounted: function() {
            this.canvas = new fabric.Canvas('paint', {
                isDrawingMode : true,
                width: 560,
                height: 560
            });
            this.canvas.freeDrawingBrush.width = 20;
            this.canvas.freeDrawingBrush.color = 'red';
        }
    };
</script>

<style scoped>
    
</style>