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
    .controller('DateTimeController', function ($http) {
        var dateTime = this;
        $http.get('/date-time.json').success(function (data) {
            dateTime.data = data;
        });
    })
    .controller('WateringController', function ($http) {
        var watering = this;
        $http.get('/watering-alarms.json').success(function (alarms) {
            watering.alarms = alarms;
        });
    });
