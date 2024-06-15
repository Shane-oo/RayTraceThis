using Carter;
using RayTraceThis.Net.WebApi;

var builder = WebApplication.CreateBuilder(args);


var appSettings = builder.Configuration.GetSection(AppSettings.SECTION_NAME)
                         .Get<AppSettings>();

builder.Services.AddOptions<AppSettings>()
       .BindConfiguration(AppSettings.SECTION_NAME)
       .ValidateDataAnnotations()
       .ValidateOnStart();

const string CORS_POLICY_NAME = "CorsPolicy";
builder.Services.AddCors(corsOptions =>
                         {
                             corsOptions.AddPolicy(CORS_POLICY_NAME, p =>
                                                                     {
                                                                         p.AllowAnyMethod()
                                                                          .AllowAnyHeader()
                                                                          .AllowCredentials()
                                                                          .WithOrigins(appSettings.ClientUrls);
                                                                     });
                         });

builder.Services.AddDataContext(builder.Configuration);

// todo  builder.Services.AddAuthorizationBuilder();

builder.Services.AddOpenIddictServer(builder.Environment);

builder.Services.AddUserIdentity();

// todo builder.Services.AddAuthentication()

builder.Services.AddDbQueries();

builder.Services.AddFluentValidators();

builder.Services.AddMediatRServices();

builder.Services.AddCarter();

var app = builder.Build();

// Configure the HTTP request pipeline.
if (!app.Environment.IsDevelopment())
{
    app.UseHsts();
}

app.UseHttpsRedirection();

app.UseRouting();
app.UseCors(CORS_POLICY_NAME);

// todo app.UseAuthentication();
// todo app.UseAuthorization();

//todo app.UseMiddleware<ExecptionHandling>();

// Map all Minimal Api Endpoints that inherit ICarterModule/CarterModule
app.MapCarter();

app.Run();
