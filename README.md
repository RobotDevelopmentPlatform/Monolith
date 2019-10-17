# Monolith drive

### Wysokopoziomowe funkcje do sterowania każdym z silników:
```
void drive_FL(float power);       // Lewy, przedni silnik - wartość w procentach w przedziale [-100% do +100%]
void drive_RL(float power);       // Lewy, tylni silnik - wartość w procentach w przedziale [-100% do +100%]
void drive_FR(float power);       // Prawy, przedni silnik - wartość w procentach w przedziale [-100% do +100%]
void drive_RR(float power);       // Prawy, tylny silnik - wartość w procentach w przedziale [-100% do +100%]
```

### Maksymalną moc silników (czyli tą dla wartości _power = +-100%_) można zmienić tutaj:
`#define MAX_POWER 500` - zmiana w zakresie 0 do 1000

Do poprawnego działania funkcji potrzebny jest skonfigurowany **timer1** w taki sposób, jak to jest zrobione w plikach _tim.c/tim.h_
