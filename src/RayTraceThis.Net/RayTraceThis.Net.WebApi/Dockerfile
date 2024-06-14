FROM mcr.microsoft.com/dotnet/aspnet:8.0 AS base
USER $APP_UID
WORKDIR /app
EXPOSE 8080
EXPOSE 8081

FROM mcr.microsoft.com/dotnet/sdk:8.0 AS build
ARG BUILD_CONFIGURATION=Release
WORKDIR /src
COPY ["RayTraceThis.Net.WebApi/RayTraceThis.Net.WebApi.csproj", "RayTraceThis.Net.WebApi/"]
RUN dotnet restore "RayTraceThis.Net.WebApi/RayTraceThis.Net.WebApi.csproj"
COPY . .
WORKDIR "/src/RayTraceThis.Net.WebApi"
RUN dotnet build "RayTraceThis.Net.WebApi.csproj" -c $BUILD_CONFIGURATION -o /app/build

FROM build AS publish
ARG BUILD_CONFIGURATION=Release
RUN dotnet publish "RayTraceThis.Net.WebApi.csproj" -c $BUILD_CONFIGURATION -o /app/publish /p:UseAppHost=false

FROM base AS final
WORKDIR /app
COPY --from=publish /app/publish .
ENTRYPOINT ["dotnet", "RayTraceThis.Net.WebApi.dll"]
