# Monolith drive

### Wysokopoziomowe funkcje do sterowania każdym z silników:
```
void drive_FL(float power);       // Lewy, przedni silnik - wartość w procentach w przedziale [-100% do +100%]
void drive_RL(float power);       // Lewy, tylni silnik - wartość w procentach w przedziale [-100% do +100%]
void drive_FR(float power);       // Prawy, przedni silnik - wartość w procentach w przedziale [-100% do +100%]
void drive_RR(float power);       // Prawy, tylny silnik - wartość w procentach w przedziale [-100% do +100%]
```

### Zatrzymanie silnika:
Aby zatrzymać silnik należy wywołać odpowiednią funkcję z wartością `power = 0`.
Przykładowo:
```
void drive_FL(0);
void drive_RL(0);  
void drive_FR(0);       
void drive_RR(0);
```

### Maksymalną moc silników (czyli tą dla wartości _power = +-100%_) można zmienić tutaj:
`#define MAX_POWER 500` - zmiana w zakresie 0 do 1000

Do poprawnego działania funkcji potrzebny jest skonfigurowany **timer1** w taki sposób, jak to jest zrobione w plikach _tim.c/tim.h_
