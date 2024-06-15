import {Component, computed, DestroyRef, effect, inject, OnInit, signal, WritableSignal} from '@angular/core';
import {RouterOutlet} from '@angular/router';
import {AppService, WeatherForecast} from "./app.service";
import {takeUntilDestroyed} from "@angular/core/rxjs-interop";

interface UserInterface {
  id: string;
  name: string;
}

@Component({
  selector: 'app-root',
  standalone: true,
  imports: [RouterOutlet],
  templateUrl: './app.component.html',
  styleUrl: './app.component.css'
})
export class AppComponent implements OnInit {
  destroyRef = inject(DestroyRef);
  title: WritableSignal<string> = signal('');
  users = signal<UserInterface[]>([{
    id: '0',
    name: 'Bar'
  }]);
  titleChangeEffect = effect(() => {
    console.log('titleChangeEffect', this.title());
  });
  usersTotal = computed(() => this.users().length);

  weather = signal<WeatherForecast[]>([]);
  weatherChangeEffect = effect(() => {
    console.log('weatherChangeEffect', this.weather());
  });
  weatherTotal = computed(()=>this.weather().length);

  constructor(private readonly appService: AppService) {
  }

  ngOnInit(): void {
    setTimeout(() => {
      this.users.update(prevUsers => [...prevUsers, {
        id: '1',
        name: 'Foo'
      }]);
    }, 2000);
  }

  changeTitle(event: Event) {
    const title = (event.target as HTMLInputElement).value;

    this.title.set(title);
  }

  getWeather() {
    this.appService.WeatherForecast()
      .pipe(takeUntilDestroyed(this.destroyRef))
      .subscribe((weather) => {
        this.weather.set(weather);
      });
  }

  addToWeather() {
    this.weather.update(prevWeather => [...prevWeather, {
      date: '2024-06-21',
      temperatureC: 0,
      summary: 'I Faked this one',
      temperatureF: 0
    }]);
  }
}
