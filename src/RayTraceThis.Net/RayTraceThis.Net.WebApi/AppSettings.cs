using System.ComponentModel.DataAnnotations;

namespace RayTraceThis.Net.WebApi;

public class AppSettings
{
    #region Constants

    public const string SECTION_NAME = "AppSettings";

    #endregion

    #region Properties

    [Required]
    public string[] ClientUrls { get; init; }

    #endregion
}
