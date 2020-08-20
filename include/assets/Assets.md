Asset Manager
=============

The Asset Manager loads each file in the `/assets/` directory of the game's CWD as an `Asset`, and if the extension matches a known asset type, an instance of the native type is also initialized and stored in the `Asset::_Asset` member, and can be retreived using the `Asset::GetAsset<TAsset>()` method, providing the appropriate asset type for `TAsset`, given in the following table.

These are the known asset types, in the order that they are initialized:

| Extension |      Encoding      |    Native Type     |                Notes                |
|-----------|--------------------|--------------------|-------------------------------------|
| `.glsl`   | GLSL shader source | `nullptr`          | Just use the raw data in the asset. |
| `.png`    | PNG data           | `GLTexture`        |                                     |
| `.pva`    | JSON               | `SpriteAnimation`  |                                     |

All native classes for assets are expected to take a constructor with the following signature: `void (const char* data, size_t dataLength)`.

Please avoid creating instances of assets that the Asset Manager already has. (i.e. Don't create a `GLTexture` yourself from data in an asset; get the Asset Manager's `GLTexture` for that asset via `Asset::GetAsset()` instead.)