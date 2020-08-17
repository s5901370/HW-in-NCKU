import Vue from 'vue';
import Vuetify from 'vuetify';
import 'vuetify/dist/vuetify.min.css';
import App from './app.vue';

Vue.use(Vuetify);

new Vue({
    el: '#app',
    components: {
        App
    },
    
    render: (createElement) => createElement(App)
});