namespace RayTraceThis.Net.Core.Exchange;

public class Error
{
    #region Fields

    public static readonly Error None = new(string.Empty, string.Empty);

    public static readonly Error NullValue = new("NullValue", "Value cannot be null");

    public static readonly Error Unknown = new("Unknown", "Something went wrong");

    #endregion

    #region Properties

    public string Code { get; }

    public string Description { get; }

    #endregion

    #region Construction

    public Error(string code, string description)
    {
        Code = code;
        Description = description;
    }

    #endregion

    #region Public Methods

    public static Error NotFound(string param)
    {
        return new Error("NotFound", $"{param} was not found");
    }

    #endregion
}
