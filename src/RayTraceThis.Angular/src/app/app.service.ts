import {Injectable} from '@angular/core';
import {HttpClient} from "@angular/common/http";

export interface WeatherForecast {
  date: string;
  temperatureC: number;
  summary: string;
  temperatureF: number;
}

@Injectable({
  providedIn: 'root'
})
export class AppService {

  constructor(private readonly http: HttpClient) {
  }

  WeatherForecast() {
    return this.http.get<WeatherForecast[]>('https://localhost:7076/weatherforecast');
  }
}
