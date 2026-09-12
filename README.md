# AvirA IL2CPP Resolver

Легкая обертка над рантаймом IL2CPP для читов под Unity игры. Резолвишь классы, методы и поля по именам, получаешь готовые указатели на функции и вызываешь их одной строкой. Никаких сигнатур и офсетов руками.

Классы с префиксом `C_`, методы в PascalCase, файлы парами hpp/cpp, весь код в неймспейсе `AvirA`. Комментариев в коде нет специально, вся документация здесь.

## Что нужно

Windows x64, MSVC v143, C++17. Других зависимостей нет, только Windows SDK. Проект собирается как статическая библиотека, CMake и Clang не используются.

## Как впихнуть в свой проект

Есть три способа, выбирай любой.

Первый, самый простой: скопируй папку `src` к себе в проект и подключи один заголовок:

```cpp
#include "Resolver.hpp"
```

Второй: добавь репозиторий гит сабмодулем и подключи исходники:

```sh
git submodule add https://github.com/ANTONSVD/AvirA-IL2CPP-Resolver
```

Третий: открой `AvirAResolver.sln`, собери Release x64, забери `out/AvirAResolver.lib` и линкуй его, а из исходников нужны только заголовки.

## Быстрый старт

Создаешь объект резолвера и вызываешь `Initialize`. Обычно это делается один раз в `DllMain` или в потоке инициализации чита. Объект можно держать где удобно, глобальных переменных внутри библиотеки нет.

```cpp
#include "Resolver.hpp"

AvirA::C_Resolver Resolver;

bool Startup()
{
    if (!Resolver.Initialize("GameAssembly.dll", true, 60))
        return false;
    return true;
}
```

Первый аргумент это имя модуля, второй говорит ждать ли его загрузку если твой DLL встал раньше игры, третий сколько секунд ждать максимум. После этого можно резолвить.

Поток, из которого вызываешь методы игры через `Invoke`, должен быть приаттачен к домену. Прямые вызовы через указатель аттача не требуют. Как аттачить написано ниже в разделе про потоки.

## Главная фишка: резолв метода в одну строку

Главная фишка это резолв метода в одну строку. Объявляешь тип функции, получаешь указатель, вызываешь как обычную функцию:

```cpp
using IsAliveFn = bool(*)(AvirA::RawObject*);

IsAliveFn IsAlive = Resolver.Function<IsAliveFn>(
    "Assembly-CSharp", "", "CharacterClassManager", "get_IsAlive", 0);

bool Alive = IsAlive(LocalPlayer->Raw());
```

`C_Object` это вид на объект игры, а не сам объект, поэтому в прямые вызовы передается `Raw()`. Все обертки вида `Call` и `Get` разворачивают его сами, руками это нужно только для тайпдефов.

Порядок аргументов: сборка, неймспейс, класс, метод, число параметров. Число параметров можно не указывать, по умолчанию стоит -1, это значит любой. Пустой неймспейс это просто `""`.

Если адрес не нашелся, вернется `nullptr`, а не краш. Проверяй указатель перед использованием в важных местах.

## Классы

Класс лучше закешировать один раз, а дальше брать от него все что нужно:

```cpp
AvirA::C_Class PlayerClass = Resolver.ResolveClass("Assembly-CSharp", "", "PlayerHandler");

if (!PlayerClass.Valid())
    return;
```

Если не знаешь в какой сборке лежит класс, ищи по полному имени с точкой через все сборки сразу:

```cpp
AvirA::C_Class CameraClass = Resolver.FindClass("UnityEngine.Camera");
```

У класса есть информация о нем самом:

```cpp
std::string Name = PlayerClass.Name();
std::string Ns = PlayerClass.Namespace();
std::string Asm = PlayerClass.AssemblyName();
bool Generic = PlayerClass.IsGeneric();
bool Inflated = PlayerClass.IsInflated();
bool Value = PlayerClass.IsValuetype();
bool Enum = PlayerClass.IsEnum();
AvirA::C_Class Parent = PlayerClass.Parent();
bool Child = PlayerClass.IsSubclassOf(Parent);
```

Вложенный класс достается по имени:

```cpp
AvirA::C_Class Inventory = PlayerClass.Nested("Inventory");
```

## Методы

Метод от кешированного класса, короткая запись без сборки и неймспейса:

```cpp
AvirA::C_Method Update = PlayerClass.Method("Update");
void* Pointer = Update.Pointer();

using UpdateFn = void(*)(AvirA::RawObject*);
UpdateFn Fn = PlayerClass.Resolve<UpdateFn>("Update", 0);
```

Вызов без объявления типа, прямо по объекту:

```cpp
PlayerClass.Call<void>(Player, "SetActive", 1, true);
bool Alive = PlayerClass.Call<bool>(Player, "get_IsAlive", 0);
```

Перебрать все методы класса и посмотреть их параметры:

```cpp
for (AvirA::C_Method M : PlayerClass.Methods())
{
    std::string Name = M.Name();
    AvirA::u32 Count = M.ParamCount();
    std::string First = M.ParamName(0);
    std::string TypeName = M.ParamType(0).Name();
    bool Static = M.IsStatic();
}
```

Все перегрузки метода с одним именем:

```cpp
std::vector<AvirA::C_Method> All = PlayerClass.Overloads("SetActive");
```

## Перегрузки по типам параметров

Когда у метода несколько перегрузок и число аргументов не помогает, выбирай по именам типов параметров:

```cpp
void* Pointer = Resolver.Overload(PlayerClass, "Teleport", { "UnityEngine.Vector3", "System.Boolean" });
```

Сравнение идет по именам классов типов, работает на любых версиях Unity.

## Вызов через рантайм

Прямой вызов по указателю это самый быстрый путь. Но иногда нужен `il2cpp_runtime_invoke`, например для динамических случаев. Для этого есть `Invoke`, он возвращает упакованный результат:

```cpp
AvirA::C_Method GetHealth = PlayerClass.Method("GetHealth");
void* Args[] = { nullptr };
AvirA::RawObject* Boxed = GetHealth.Invoke(Player->Raw(), Args);
```

Если метод возвращает значимый тип, распакуй его сразу:

```cpp
int Health = GetHealth.InvokeValue<int>(Player->Raw());
int Damage = TakeDamage.InvokeValue<int>(Player->Raw(), 10, true);
```

Для вызова нужен приаттаченный поток, иначе игра упадет. Про аттач читай раздел про потоки.

Виртуальный метод для конкретного объекта:

```cpp
AvirA::C_Method Base = PlayerClass.Method("Update");
AvirA::C_Method Real = Base.Virtual(Player);
void* Pointer = Real.Pointer();
```

## Поля объекта

Чтение и запись полей по имени:

```cpp
AvirA::C_Field Health = PlayerClass.Field("health");
int Hp = Health.Get<int>(Player);
Health.Set(Player, 100);
```

Еще короче, прямо через объект без отдельного резолва поля:

```cpp
int Hp = Player->Get<int>("health");
Player->Set("health", 100);
```

Если поля с таким именем нет, а есть проперти, `Get` и `Set` сами позовут геттер или сеттер. Отдельно руками это тоже можно:

```cpp
AvirA::C_Property Transform = PlayerClass.Property("transform");
AvirA::C_Object T = Player->GetObj("transform");
```

Объекты читаются через `GetObj`, он возвращает вид по значению:

```cpp
AvirA::C_Object Model = Player->GetObj("MyModel");
AvirA::Vector3 Pos = Player->Get<AvirA::Vector3>("position");
```

Сырой адрес поля в объекте, если нужно поковырять руками:

```cpp
void* Address = Health.Address(Player);
```

Перебрать все поля класса с офсетами и флагами:

```cpp
for (AvirA::C_Field F : PlayerClass.Fields())
{
    std::string Name = F.Name();
    size_t Offset = F.Offset();
    bool Static = F.IsStatic();
    bool Pub = F.IsPublic();
}
```

## Статические поля и методы

Статика читается и пишется в одну строку:

```cpp
AvirA::RawObject* Instance = Resolver.Static<AvirA::RawObject*>(
    "Assembly-CSharp", "", "PlayerHandler", "Instance");

Resolver.SetStatic("Assembly-CSharp", "", "PlayerHandler", "Instance", Instance);
```

То же самое через кешированный класс:

```cpp
AvirA::C_Field Inst = PlayerClass.Field("Instance");
AvirA::RawObject* P = Inst.Static<AvirA::RawObject*>();
Inst.SetStatic(P);
```

Перед чтением статики библиотека сама дергает инициализацию класса, руками ничего делать не нужно.

Статический метод вызывается как обычно, только объект не передается:

```cpp
using GetLocalFn = AvirA::RawObject*(*)();
GetLocalFn GetLocal = Resolver.Function<GetLocalFn>(
    "Assembly-CSharp", "", "ReferenceHub", "get_LocalHub", 0);

AvirA::C_Object Local(Resolver.Api(), GetLocal());
```

## Обобщения (генерики)

Найти закрытый вариант generic класса по аргументам:

```cpp
AvirA::C_Class ListDef = Resolver.FindClass("System.Collections.Generic.List`1");
AvirA::C_Class IntClass = Resolver.FindClass("System.Int32");
AvirA::C_Class IntList = Resolver.Inflate(ListDef, { IntClass });
```

Результат кешируется внутри резолвера, повторный вызов ничего не ищет заново.

Посмотреть аргументы уже закрытого generic класса:

```cpp
AvirA::u32 Count = IntList.GenericArgCount();
AvirA::C_Class Arg = IntList.GenericArgAt(0);
```

Инфлейт ищется перебором классов домена и сравнением аргументов, так что работает без приватных экспортов.

## Создание объектов, бокс, строки

Создать объект класса, конструктор вызовется сам:

```cpp
AvirA::C_Object Obj = PlayerClass.Create();
```

Упаковать значимый тип в объект и распаковать обратно:

```cpp
int Value = 5;
AvirA::C_Object Boxed = Resolver.Box(IntClass, &Value);
int Back = Boxed.Unbox<int>();
```

Строки:

```cpp
AvirA::C_String S = Resolver.NewString("hello");
std::string Text = S.Text();
int Len = S.Length();
```

## Массивы, листы, словари

Массивы создаются через резолвер, читаются с проверкой границ:

```cpp
AvirA::C_Array<AvirA::C_Object*> Items = Resolver.NewArray<AvirA::C_Object*>(ItemClass, 10);
AvirA::u32 Count = Items.Length();
Items.Set(0, FirstItem->Raw());
AvirA::C_Object Got = Items.GetObj(0);
```

Для массивов с объектами бери `GetObj` вместо `Get`, он правильно завернет указатель в вид.

Список `List<T>` оборачивается поверх объекта:

```cpp
AvirA::C_List<AvirA::C_Object*> Inv(Player->GetObj("inventory"));
int Count = Inv.Count();
AvirA::C_Object First = Inv.AtObj(0);
```

Для списков с объектами бери `AtObj` вместо `At`, по той же причине.

Словарь `Dictionary<K, V>` ходит через настоящие методы `TryGetValue` и `ContainsKey`, поэтому не зависит от версии игры:

```cpp
AvirA::C_Dictionary<int, AvirA::C_Object*> Dict(Obj);
int Count = Dict.Count();
if (Dict.Contains(1))
{
    AvirA::C_Object Item = Dict.GetObj(1);
}
```

Для словарей с объектами бери `GetObj`, обычный `Get` вернет голый указатель без обертки.

Математика Unity лежит в `src/Unity/Math.hpp`: `Vector2`, `Vector3`, `Vector4`, `Quaternion`, `Color`, `Rect`, `Bounds`, `Ray`, `Matrix4x4`. У вектора есть длина, дистанция и скалярное произведение.

## Потоки

Для `Invoke` текущий поток должен быть приаттачен. Проще всего обернуть кусок кода в скоп, аттач и детач произойдут сами:

```cpp
{
    AvirA::C_AttachScope Scope(Resolver.Api(), Resolver.Domain());
    int Health = GetHealth.InvokeValue<int>(Player->Raw());
}
```

Свой поток с аттачем:

```cpp
AvirA::C_Thread::Spawn(Resolver.Api(), Resolver.Domain(), MyFeature, MyCleanup);
```

Вручную:

```cpp
void* Thread = AvirA::C_Thread::Attach(Resolver.Api(), Resolver.Domain());
AvirA::C_Thread::Detach(Resolver.Api(), Thread);
```

## Хуки методов

Указатель метода это готовая цель для MinHook:

```cpp
#define UFUNC(P) (*(void**)(P))

void* Target = PlayerClass.Method("Update").Pointer();
MH_CreateHook(Target, &HkUpdate, (void**)&OUpdate);
```

Для хуков без MinHook есть работа с vtable. Найти слот по первым байтам функции и подменить:

```cpp
void** Table = *(void***)SomeObject;
void** Slot = AvirA::C_Vtable::Find(Table, 100, { 0x33, 0xD2, 0xE9 });
void* Original = nullptr;
AvirA::C_Vtable::Replace(Slot, &MyUpdate, &Original);
```

## Поиск скрытого под обфускацией

Когда имена классов зашифрованы, ищи класс по набору полей и методов. Тильда значит поле, минус значит метод, без знака проверяется и то и другое:

```cpp
AvirA::C_Class Hidden = Resolver.FilterAll("Assembly-CSharp", "", { "~_health", "-Update" });
```

Можно искать по конкретному имиджу или по готовому списку классов:

```cpp
AvirA::C_Image Img = Resolver.Image("Assembly-CSharp");
AvirA::C_Class Hidden = Resolver.FilterImage(Img, { "~_health", "-Update" });
```

## Прочее полезное

Айколлы, пин объектов от сборщика мусора, проверка отладчика, сборка мусора:

```cpp
void* Icall = Resolver.Icall("UnityEngine.GameObject::Internal_CreateGameObject");
AvirA::u32 Handle = Resolver.Pin(&Obj);
AvirA::C_Object Target = Resolver.PinnedTarget(Handle);
Resolver.Unpin(Handle);
bool Debugged = Resolver.DebuggerAttached();
Resolver.GcCollect();
```

Кешированные системные типы, чтобы не искать `System.Int32` каждый раз:

```cpp
AvirA::C_Type IntType = Resolver.SystemType("System.Int32");
AvirA::C_Class IntClass = IntType.Class();
```

Если какой то экспорт не зарезолвился, посмотри какие именно:

```cpp
for (std::string& Name : Resolver.MissingExports())
{
}
```

Резолв экспортов сначала пробует имя как есть, потом перебирает ROT сдвиги от 1 до 25. Это покрывает игры с обфусцированной таблицей экспортов.

## Структура проекта

```
AvirAResolver.sln      солюшен библиотеки
AvirAResolver.vcxproj  сама библиотека, собирается в out/AvirAResolver.lib
src/
  AvirA.hpp            типы u8/u32/u64 и версия
  Raw.hpp              минимальные структуры il2cpp
  Api.hpp/Api.cpp      таблица экспортов, загрузка из GameAssembly, ROT перебор
  Domain.hpp/cpp       домен, сборки, поиск класса по полному имени
  Image.hpp/cpp        образ сборки, список классов
  Class.hpp/cpp        класс, методы, поля, вложенные классы, генерики
  Method.hpp/cpp       метод, указатель, параметры, invoke
  Field.hpp/cpp        поле, офсет, чтение и запись, статика
  Property.hpp/cpp     проперти, геттеры и сеттеры
  Object.hpp/cpp       объект, Get/Set/Call, obscured значения
  String.hpp/cpp       строки il2cpp
  Type.hpp/cpp         типы, имена без утечек
  Array.hpp            массивы, только заголовок
  Thread.hpp/cpp       аттач потока, скоп, создание потока
  Vtable.hpp/cpp       поиск и подмена в vtable
  Resolver.hpp/cpp     фасад, короткие однострочные резолвы, кеши
  Inline.hpp           тела шаблонов, подключается сам
  Unity/Math.hpp       вектора, кватернионы, цвета
  Unity/List.hpp       обертка List<T>
  Unity/Dictionary.hpp обертка Dictionary<K, V>
```

## Нюансы

Библиотека только под x64 Windows. Весь API терпит null: незарезолвленный класс, метод или поле просто вернут пустое значение вместо падения. Но это не повод не проверять `Valid()` в важных местах.

Библиотека работает на любых версиях Unity без дефайнов под конкретную версию. Из структур il2cpp напрямую читаются только поля со стабильным положением, все остальное идет через экспорты рантайма.

Адрес `GameAssembly.dll` и его размер доступны через `ModuleBase()` и `ModuleSize()`, пригодится для своих сканов.

## Лицензия

MIT, файл LICENSE в корне. Делай что хочешь, только сохрани копирайт.
