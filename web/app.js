angular.module('project', ['ngRoute'])
    .config(
        function ($routeProvider) {
            $routeProvider
                .when('/', {
                    templateUrl: 'tiles.html'
                })
                .when('/watering', {
                    templateUrl: 'watering.html'
                })
                .otherwise({
                    redirectTo: '/'
                });
        }
    )
    .controller('DateTimeController', function () {
        this.data = {
            date: "20 Апреля",
            time: "20:12",
            temperature: "0"
        };
    })
    .controller('WateringController', function () {
        this.alarms = [
            {time: "08:00"},
            {time: "12:00"},
            {time: "21:00"}
        ];
    });
